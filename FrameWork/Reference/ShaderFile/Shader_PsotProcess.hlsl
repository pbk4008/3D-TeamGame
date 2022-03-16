#include "Shader_Calculate.hpp"

//#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};
sampler SamplerWrap = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer ShaderCheck
{
	bool g_bShadow;
};

cbuffer ConstBuffer
{
	vector	g_BrightPassOffset[4];
	vector	g_DSOffset[16];
	float	g_BrightPassThreshold;
};

cbuffer HorizontalBlur
{
	float g_HBloomOffset[9];
	float g_HBloomWeight[9];
};

cbuffer VerticalBlur
{
	float g_VBloomOffset[9];
	float g_VBloomWeight[9];
};

texture2D g_Basetexture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;
	
	Out.vPosition = vector(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vOutColor	: SV_Target0;
};

PS_OUT PS_MAIN_BrightPass(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	float4 Average = float4(0.f, 0.f, 0.f, 0.f);
	
	for (int i = 0; i < 4; i++ )
	{
		Average += g_Basetexture.Sample(DefaultSampler, (In.vTexUV + float2(g_BrightPassOffset[i].x, g_BrightPassOffset[i].y)));
	}
	Average *= 0.25f;
	
	float Luminance = max(Average.r, max(Average.g, Average.b));
	
	if(Luminance < g_BrightPassThreshold)
		Average = float4(0.f, 0.f, 0.f, 1.f);
		
	Out.vOutColor = Average;
	
	return Out;
}

PS_OUT PS_MAIN_DOWNSAMPLE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	float4 Average = float4(0.f, 0.f, 0.f, 0.f);
	
	for (int i = 0; i < 16 ; i++)
	{
		Average += g_Basetexture.Sample(DefaultSampler, In.vTexUV + float2(g_DSOffset[i].x, g_DSOffset[i].y));
	}
	Average *= (1.f / 16.f);
	Out.vOutColor = Average;
	
	return Out;
}

PS_OUT PS_MAIN_HorizontalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 BaseColor = float4(0.f, 0.f, 0.f, 0.f);
	
	for (int i = 0; i < 9; i++)
	{
		BaseColor += (g_Basetexture.Sample(DefaultSampler, (In.vTexUV + float2(g_HBloomOffset[i], 0.f))) * g_HBloomWeight[i]);
	}
	Out.vOutColor = vector(BaseColor.rgb, 1.f);
	
	return Out;
}

PS_OUT PS_MAIN_VerticalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 BaseColor = float4(0.f, 0.f, 0.f, 0.f);
	
	for (int i = 0; i < 9; i++)
	{
		BaseColor += (g_Basetexture.Sample(DefaultSampler, (In.vTexUV + float2(0.f, g_VBloomOffset[i]))) * g_VBloomWeight[i]);
	}
	Out.vOutColor = vector(BaseColor.rgb, 1.f);
	
	return Out;
}



//--------------------------------------------------------------------------------------------------------------------------//
technique11 PostProcess
{
	pass BrightPass
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BrightPass();
	}

	pass BrightPassDownSample
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOWNSAMPLE();
	}

	pass HorizontalBlur
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HorizontalBlur();
	}

	pass VerticalBlur
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VerticalBlur();
	}
	
}
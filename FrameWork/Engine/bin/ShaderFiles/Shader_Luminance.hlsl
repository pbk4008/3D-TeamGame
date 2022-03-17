#include "Shader_Calculate.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

cbuffer ShaderCheck
{
	bool g_bShadow;
};

cbuffer ConstBuffer
{
	vector g_GrayOffset[4];
	vector g_DSOffset[9];
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
	vector vLuminanceTex	: SV_Target0;
};

PS_OUT PS_MAIN_GRAYDOWNSAMPLE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	
	float fAverage = 0.f;
	float fMaximum = -1e20;
	vector BaseColor = 0.f;
	for (int i = 0; i < 4; ++i)
	{
		BaseColor = g_Basetexture.Sample(DefaultSampler, In.vTexUV + float2(g_GrayOffset[i].x, g_GrayOffset[i].y));
		float grayvalue = max(BaseColor.r, max(BaseColor.g, BaseColor.b));
		fMaximum = max(fMaximum, grayvalue);
		fAverage += (0.25f * log(1e-5 + grayvalue));
	}
	fAverage = exp(fAverage);
	Out.vLuminanceTex = vector(fAverage, fMaximum, 0.f, 1.f);
	
	return Out;
}
PS_OUT PS_MAIN_DOWNSAMPLE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	vector Basecolor = 0.0f;
	float fMaximum = -1e20;
	float fAverage = 0.0f;
	
	for (int i = 0; i < 9; ++i)
	{
		Basecolor = g_Basetexture.Sample(DefaultSampler, In.vTexUV + float2(g_DSOffset[i].x, g_DSOffset[i].y));
		fAverage += Basecolor.r;
		fMaximum = max(fMaximum, Basecolor.g);
	}
	fAverage /= 9.0f;
	Out.vLuminanceTex = vector(fAverage, fMaximum, 0.f, 1.f);
	
		return Out;
}

PS_OUT PS_MAIN_DSDB(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	vector Luminancne = g_Basetexture.Sample(DefaultSampler, In.vTexUV);
	
	float scalar = 2.0f;
	
	Out.vLuminanceTex = vector(Luminancne.r * scalar, Luminancne.r * scalar, Luminancne.r * scalar, 1.f);
	
	return Out;
}



//--------------------------------------------------------------------------------------------------------------------------//
technique11 Luminance
{
	pass GrayDownSample
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GRAYDOWNSAMPLE();
	}
	pass DownSample
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOWNSAMPLE();
	}
	pass DBLuminance
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DSDB();
	}
	

}
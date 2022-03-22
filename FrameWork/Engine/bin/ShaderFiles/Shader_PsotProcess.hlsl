#include "Shader_Calculate.hpp"

sampler DefaultSampler = sampler_state
{
	filter = anisotropic;
	MaxAnisotropy = 16;

	AddressU = mirror;
	AddressV = mirror;

	//filter = min_mag_mip_linear;
	//AddressU = wrap;
	//AddressV = wrap;
};

cbuffer RtPixel
{
	float2 g_RtperPixel;
};

texture2D g_Basetexture;

int g_blurSize = 3;

float pixelKernel[13] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
};

float blurWeights[13] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};


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
	vector vOutColor : SV_Target0;
};

PS_OUT PS_MAIN_VerticalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 color = 0;
	for (int i = 0; i < 13; ++i)
	{
		color += g_Basetexture.Sample(DefaultSampler, float2(
													In.vTexUV.x + g_RtperPixel.x,
													In.vTexUV.y + (pixelKernel[i] * g_RtperPixel.y * g_blurSize) + g_RtperPixel.y))
													* blurWeights[i];
	}
	
	Out.vOutColor = color;
	
	return Out;
}

PS_OUT PS_MAIN_HorizontalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 color = 0;
	for (int i = 0; i < 13; ++i)
	{
		color += g_Basetexture.Sample(DefaultSampler, float2(
													In.vTexUV.x + (pixelKernel[i] * g_RtperPixel.x * g_blurSize) + g_RtperPixel.x,
													In.vTexUV.y + g_RtperPixel.y))
													* blurWeights[i];
	}
	
	Out.vOutColor.xyz = color.xyz;
	
	return Out;
}


//--------------------------------------------------------------------------------------------------------------------------//
technique11 PostProcess
{
	pass VerticalBlur
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VerticalBlur();
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
	
}
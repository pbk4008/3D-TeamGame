#include "Shader_Calculate.hpp"

sampler SkySampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler SkyBoxSampler = sampler_state
{
	AddressU = mirror;
	AddressV = mirror;
};

cbuffer ConstBuffer
{
	float4	g_sspos;
	float3	g_lightpos;
	float	g_raydensity;
	float	g_rayweight;
	float	g_raydecay;
	float	g_rayexposure;
	
};

Texture2D g_SkyTexture;

static const int NUM_SAMLES = 32;

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

PS_OUT PS_MAIN_GODRAY(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float2 texcoord = In.vTexUV;
	float3 color = g_SkyTexture.SampleLevel(SkySampler, texcoord, 0).rgb;
	
	float2 lightpos = g_sspos.xy;
	
	float2 deltatexcoord = (texcoord - lightpos);
	deltatexcoord *= g_raydensity / NUM_SAMLES;
	
	float illuminationdecay = 1.0f;
	
	float3 accgodray = float3(0.0f, 0.0f, 0.0f);
	
	float3 acc = 0.f;
	
	for (int i = 0; i < NUM_SAMLES; i++)
	{
		texcoord.xy -= deltatexcoord;
		float3 sam = g_SkyTexture.SampleLevel(SkySampler, texcoord.xy, 0).rgb;
		sam *= illuminationdecay * g_rayweight;
		acc += sam;
		illuminationdecay *= g_raydecay;
	}
	
	acc *= g_rayexposure;
	
	Out.vOutColor = float4(color + acc, 1.f);
	
	return Out;
}


//--------------------------------------------------------------------------------------------------------------------------//
technique11 PostProcess
{
	pass GodRay
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GODRAY();
	}
}
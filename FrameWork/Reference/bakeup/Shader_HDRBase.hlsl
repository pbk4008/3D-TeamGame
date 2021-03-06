#include "Shader_Calculate.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler DiffuseSampler = sampler_state
{
	filter = anisotropic;
	MaxAnisotropy = 16;
};

sampler ShadowSampler = sampler_state
{
	filter = anisotropic;
	MaxAnisotropy = 16;
};

cbuffer ShaderCheck
{
	bool g_bShadow;
};

cbuffer ConstBuffer
{
	//float Gamma = 2.2f;
	//float LightAmount = 1.f;
	//float SpecularAmount = 1.f;
};

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_SpecularTexture;
texture2D g_ShadeTexture;
texture2D g_SSS;

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
	vector vHDRDiffuse : SV_TARGET0;
	vector vHDRSpecular : SV_Target1;
};

PS_OUT PS_MAIN_HDDRBASE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);
	float3 normal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	float4 specualr = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	float4 final;
	if (any(normal))
	{
		float4 light = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
		
		final = diffuse * light;
	}
	else
	{
		final = diffuse;
	}
	
	Out.vHDRDiffuse = final;
	Out.vHDRSpecular = specualr;
	
	return Out;
}

technique11 Luminance
{
	pass HDRBase
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HDDRBASE();
	}
}
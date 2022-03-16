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
	//float Gamma = 2.2f;
	//float LightAmount = 1.f;
	//float SpecularAmount = 1.f;
};

texture2D g_ShadowTexture;
texture2D g_DiffuseTexture;
texture2D g_SpecularTexture;
texture2D g_ShadeTexture;

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
	
	vector vDiffuse, vSaecular, vShade, vShadow;
	
	float Gamma = 2.2f;
	float LightAmount = 1.f;
	float SpecularAmount = 1.3f;
	
	vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vSaecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vShade = (g_ShadeTexture.Sample(DefaultSampler, In.vTexUV));
	vShadow = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	
	if(g_bShadow)
	{
		vDiffuse *= vShadow;
		vSaecular *= vShadow;
		
		vDiffuse = pow(abs(vDiffuse), Gamma);
		vSaecular = pow(abs(vSaecular), Gamma);
	}
	else
	{
		vDiffuse = pow(abs(vDiffuse), Gamma);
		vSaecular = pow(abs(vSaecular), Gamma);
	}
	
	vDiffuse *= float4(LightAmount, LightAmount, LightAmount, 1.f);
	vSaecular *= float4(SpecularAmount, SpecularAmount, SpecularAmount, 1.f);
	
	Out.vHDRDiffuse = vDiffuse * vShade;
	Out.vHDRSpecular = vSaecular;
	
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
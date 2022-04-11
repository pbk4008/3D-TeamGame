#include "Shader_RenderState.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	//filter = anisotropic;
	//MaxAnisotropy = 16;

	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

Texture2D g_DiffuseTexture;
Texture2D g_OriginTexture;
Texture2D g_SpecularTexture;

Texture2D g_EmissionTexture;

Texture2D g_Blur2Texture;
Texture2D g_Blur4Texture;
Texture2D g_Blur8Texture;
Texture2D g_Blur16Texture;

Texture2D g_ShadowTexture;

Texture2D g_SkyBoxTexutre;

cbuffer check
{
	bool g_check;
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


PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 origindifusse = g_OriginTexture.Sample(DefaultSampler, In.vTexUV);
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	half4 emission = g_EmissionTexture.Sample(DefaultSampler, In.vTexUV);
	half4 specular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
		 
	half4 blur2 = g_Blur2Texture.Sample(DefaultSampler, In.vTexUV);
	half4 blur4 = g_Blur4Texture.Sample(DefaultSampler, In.vTexUV);
	half4 blur8 = g_Blur8Texture.Sample(DefaultSampler, In.vTexUV);
	half4 blur16 = g_Blur16Texture.Sample(DefaultSampler, In.vTexUV);
	half4 sky = g_SkyBoxTexutre.Sample(DefaultSampler, In.vTexUV);
	
	half4 emissive = ((emission) * 1.f + (blur2) * 1.3f + (blur4) * 1.5f + (blur8) * 2.5f + (blur16) * 3.5f);
	half4 final = float4(0, 0, 0, 0);
	
	if (diffuse.a <= 0)
	{
		diffuse = sky;
	}
	
	final = diffuse + specular + emissive;
	
	Out.vOutColor = final;
	
	return Out;
}

technique11 Emissionblend
{
	pass BlendEmission
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZWriteDisable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
}
#include "Shader_Calculate.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer ConstBuffer
{
	bool g_bHDR;
};

texture2D g_DiffuseTexture;
texture2D g_ShadeTexture;
texture2D g_ShadowTexture;
texture2D g_SpecularTexture;

texture2D g_ToneTexture;
texture2D g_ToneSepcular;


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
	
	vector albedo = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector Shade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector shadow = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	vector specular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	
	//vector ambient = vector(0.4f, 0.4f, 0.4f, 1.f) * albedo;
	//vector emissive = 
	vector diffuse = /*vector(1.f, 1.f, 1.f, 1.f) **/albedo * Shade;
	
	vector notTonemapping = diffuse + vector(specular.rgb, 0.f);
	
	//-----------------------------------------------------------------------//
	
	vector HDRalbedo = g_ToneTexture.Sample(DefaultSampler, In.vTexUV);
	vector HDRsepcular = g_ToneSepcular.Sample(DefaultSampler, In.vTexUV);
	
	vector Tonemapping = HDRalbedo + HDRsepcular;
	
	if (g_bHDR == true)
	{
		Out.vOutColor = Tonemapping;
		
		if(Out.vOutColor.a == 0.f)
			discard;
	}
	else
	{
		Out.vOutColor = notTonemapping;
		
		if (Out.vOutColor.a == 0.f)
			discard;
	}
	
	
	return Out;
}

technique11 Luminance
{
	pass UpSamplling
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
}
#include "Shader_Calculate.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	//filter = anisotropic;
	//MaxAnisotropy = 16;

	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

texture2D g_DiffuseTexture;
texture2D g_OriginTexture;
texture2D g_SpecularTexture;

texture2D g_EmissionTexture;

texture2D g_Blur2Texture;
texture2D g_Blur4Texture;
texture2D g_Blur8Texture;
texture2D g_Blur16Texture;
texture2D g_ShadowTexture;


texture2D g_FireTexture;

texture2D g_Fire2Texture;
texture2D g_Fire4Texture;
texture2D g_Fire8Texture;
texture2D g_Fire16Texture;



cbuffer check
{
	bool g_check;
	bool g_shadow;
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
	
	float originA = g_OriginTexture.Sample(DefaultSampler, In.vTexUV).a;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float4 emission = g_EmissionTexture.Sample(DefaultSampler, In.vTexUV);
	float4 specular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
		 
	float4 blur2 = g_Blur2Texture.Sample(DefaultSampler, In.vTexUV);
	float4 blur4 = g_Blur4Texture.Sample(DefaultSampler, In.vTexUV);
	float4 blur8 = g_Blur8Texture.Sample(DefaultSampler, In.vTexUV);
	float4 blur16 = g_Blur16Texture.Sample(DefaultSampler, In.vTexUV);
	
    float4 Fireemission = g_FireTexture.Sample(DefaultSampler, In.vTexUV);
		 
    float4 Fireblur2 = g_Fire2Texture.Sample(DefaultSampler, In.vTexUV);
    float4 Fireblur4 = g_Fire4Texture.Sample(DefaultSampler, In.vTexUV);
    float4 Fireblur8 = g_Fire8Texture.Sample(DefaultSampler, In.vTexUV);
    float4 Fireblur16 = g_Fire16Texture.Sample(DefaultSampler, In.vTexUV);
	
	float4 emissive = ((emission) * 1.f + (blur2) * 1.3f + (blur4) * 1.5f + (blur8) * 2.5f + (blur16) * 3.5f);
    float4 Fireemissive = ((Fireemission) * 1.f + (Fireblur2) * 0.2f + (Fireblur4) * 0.3f + (Fireblur8) * 0.4f + (Fireblur16) * 0.5f);
	float4 final = float4(0, 0, 0, 0);
	if (g_check == true)
	{	
		if(g_shadow == true)
		{
			float4 shadow = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
			diffuse = diffuse * shadow;
		}
		
        final.rgb = diffuse.rgb + specular.rgb + emissive.rgb + Fireemissive.rgb;
        final.a = originA + emissive.a/* + specular.a*/;
    }
	else
	{
		final = diffuse + emissive + specular;
	}
	
	Out.vOutColor = final;
	
	
		return Out;
	}

technique11 Emissionblend
{
	pass BlendEmission
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
}
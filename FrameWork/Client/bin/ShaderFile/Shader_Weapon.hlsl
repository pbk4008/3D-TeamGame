#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D	g_ShadowTexture;

Texture2D	g_DiffuseTexture;
Texture2D	g_BiNormalTexture;

Texture2D	g_MRATexture;
Texture2D	g_CEOTexture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	float3	vBiNormal : BINORMAL;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float4	vBiNormal	: BINORMAL;
	float4	vUvDepth	: TEXCOORD0;
};

VS_OUT VS_MAIN_ANIM(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	


	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
	
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));

	Out.vUvDepth.zw = Out.vPosition.zw;
	
	return Out;

}
// VS_SHADOWMAP
//*---------------------------------------------------------------------------------------------*

struct VS_OUT_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV,matWVP;
	
	vector vPosition = vector(In.vPosition, 1.f);
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);
	
	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vClipPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

// SHADOWMAP
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	float fDepth = In.vClipPos.z / In.vClipPos.w;
	
	float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	float Alpha = 1.f;
	
	if (color.r < 0.001f && color.g < 0.001f && color.b < 0.001f)
	{
		Alpha = color.r;
	}
	
	Out.vShadowDepthMap = vector(fDepth.xxx, Alpha);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT
{
	float4 diffuse	: SV_TARGET0;
	float4 normal	: SV_TARGET1;
	float4 depth	: SV_TARGET2;
	float4 mra		: SV_Target3;
	float4 emission : SV_Target4;
};

PS_OUT PS_MAIN_WEAPONE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(normal, 0);
	
	float Metalic = mra.r + g_Metalic;	
	float Roughness = mra.g + g_Roughness;
	float AO = mra.b + g_AO;
	
	float4 color = float4(1.f, 0.8431f, 0.f, 1.f);
	float4 power = 0.8f;

	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = AO;
	Out.mra.a = 1.f;
	
	Out.emission = g_color * power * ceo.g;
	return Out;	
}

technique11			DefaultTechnique
{	
	pass Weapone //------------------------------------------------------------------------------------0 Top
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WEAPONE();
	}

	pass ShadowANIM //-----------------------------------------------------------------------------------------3 Anim ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
}





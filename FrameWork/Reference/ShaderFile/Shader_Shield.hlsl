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
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float3 vTangent : TANGENT;
	float3 vBiNormal : BINORMAL;
	uint4 vBlendIndex : BLENDINDEX;
	float4 vBlendWeight : BLENDWEIGHT;
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

	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vPosition = mul(vPosition, matWVP);
	Out.vPosition = vPosition;

	vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector vBiNoraml = mul(vector(In.vBiNormal, 0.f), BoneMatrix);
	vector vTantent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vBiNoraml, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTantent, g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
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
	float3 worldpos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV, matWVP;
	
	half fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);
	
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);
	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vClipPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	
	half4 worldpos = mul(vPosition, g_WorldMatrix);
	Out.worldpos = worldpos.xyz;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

// SHADOWMAP
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADOW
{
	half4 vPosition : SV_Position;
	half2 vTexUV : TEXCOORD0;
	half4 vClipPos : TEXCOORD1;
	half3 worldpos : TEXCOORD2;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	float4 color = 1;
	float OneDividzFar = 1 / 300.f;
	color.xyz = length(In.worldpos - g_LightPos) * OneDividzFar;
	Out.vShadowDepthMap = color;
	
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

PS_OUT PS_MAIN_SHIELD(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = half4(normal, 0);
	
	half metalic = mra.r + g_Metalic;
	half roughness = mra.g + g_Roughness;
	half ao = mra.b + g_AO;
	
	Out.mra.r = metalic;
	Out.mra.g = roughness;
	Out.mra.b = ao;
	Out.mra.a = 1.f;
	Out.emission = g_color * g_empower * ceo.g;;
	
	return Out;	
}

technique11			DefaultTechnique
{	
	pass Weapone //------------------------------------------------------------------------------------0 Weapon
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHIELD();
	}

	pass ShadowANIM //-----------------------------------------------------------------------------------------1 Anim ShadowMap
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





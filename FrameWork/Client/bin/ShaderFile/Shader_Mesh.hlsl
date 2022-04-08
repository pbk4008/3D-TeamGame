#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

cbuffer ShadeCheck
{
	bool g_bool = false;
	bool g_NonTex = false;
};

Texture2D	g_DiffuseTexture;
Texture2D	g_ShadowTexture;
Texture2D	g_BiNormalTexture;
Texture2D	g_ORMTexture;
Texture2D	g_ORMETexture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	float3	vBiNormal : BINORMAL;
	uint4	vBlendIndex : BLENDINDEX;
	float4	vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
};

VS_OUT VS_MAIN_ANIM(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	


	matrix			matWV, matWVP;	

	float		fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vPosition = mul(vPosition, matWVP);

	Out.vPosition = vPosition;

	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector		vBiNoraml = mul(vector(In.vBiNormal, 0.f), BoneMatrix);
	vector		vTantent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	
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
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV,matWVP;
	
	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


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
	
	if (color.r < 0.1f && color.g < 0.1f && color.b < 0.1f)
	{
		//Alpha = color.a + 0.3f;
		discard;
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

//*---------------------------------------------------------------------------------------------*

struct PS_OUT
{
	float4 diffuse	: SV_TARGET0;
	float4 normal	: SV_TARGET1;
	float4 depth	: SV_TARGET2;
	float4 mra		: SV_Target3;
	float4 emission : SV_Target4;
};

PS_OUT PS_MAIN_ORME(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float4 orme = g_ORMETexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(normal, 0);
	
	float Metalic = orme.b;
	float Roughness = orme.g;
	float Ao = orme.r;
	
	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = Ao;
	Out.mra.a = 1.f;
	
	float4 EmissionColor = float4(1.f, 0.843f, 0.f, 1.f);
	float EmissionPower = 0.1f;
	Out.emission = float4(diffuse.xyz * orme.a * EmissionPower, 1);

	return Out;
}

PS_OUT PS_MAIN_ORM(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 orm = g_ORMTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	

	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(normal, 0);
	
	float Metalic = orm.b;
	float Roughness = orm.g;
	float Ao = orm.r;
	
	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = Ao;
	Out.mra.a = 1.f;
	
	Out.emission = g_color * g_empower;
	
	return Out;
}

technique11			DefaultTechnique
{
	pass ORME //------------------------------------------------------------------------------------0 ORME
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ORME();
	}

	pass PS_MAIN_ORM //------------------------------------------------------------------------------------1 ORM
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ORM();
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





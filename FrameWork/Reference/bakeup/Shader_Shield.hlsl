#include "Shader_RenderState.hpp"

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler ClampSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

cbuffer Matrices
{
	matrix		g_WorldMatrix  = (matrix)0;
	matrix		g_ViewMatrix;	
	matrix		g_ProjMatrix;
};

struct BoneMatrixArray
{
	matrix Bone[256];
};

cbuffer BoneMatricesBuffer
{
	BoneMatrixArray g_BoneMatrices;
};


cbuffer LightBuffer
{
	matrix g_LightView;
	matrix g_LightProj;
};

texture2D	g_ShadowTexture;

texture2D	g_DiffuseTexture;
texture2D	g_BiNormalTexture;

texture2D	g_MRATexture;
texture2D	g_CEOTexture;


float3 Normalmapping(float3 normaltex, float3x3 tbn)
{	
	normaltex = normaltex * 2 - 1;
	normaltex = normalize(normaltex);
	
	normaltex = normalize(mul(normaltex, tbn));
	normaltex = normaltex * 0.5f + 0.5f;
	
	return normaltex;
}


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


	matrix BoneMatrix = g_BoneMatrices.Bone[In.vBlendIndex.x] * In.vBlendWeight.x +
									g_BoneMatrices.Bone[In.vBlendIndex.y] * In.vBlendWeight.y +
									g_BoneMatrices.Bone[In.vBlendIndex.z] * In.vBlendWeight.z +
									g_BoneMatrices.Bone[In.vBlendIndex.w] * In.vBlendWeight.w;

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

// VS_SHADE_SHADOW
//*---------------------------------------------------------------------------------------------*
struct VS_OUT_SHADESHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vLightPosition : TEXCOORD1;
};

VS_OUT_SHADESHADOW VS_MAIN_SHADESHADOW(VS_IN In)
{
	VS_OUT_SHADESHADOW Out = (VS_OUT_SHADESHADOW) 0.f;
	
	matrix matWV, matWVP, matLightWV, matLightWVP;
	
	
	vector vPosition = vector(In.vPosition, 1.f);
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	matLightWV = mul(g_WorldMatrix, g_LightView);
	matLightWVP = mul(matLightWV, g_LightProj);
	
	Out.vPosition = mul(vector(vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLightPosition = mul(vector(vPosition.xyz, 1.f), matLightWVP);
	//Out.vLightPosition = Out.vPosition;
	
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

// SHADE SHADOW
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADESHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vLightPosition : TEXCOORD1;
};

struct PS_OUT_SHADESHADOW
{
	vector vShadeShadow : SV_TARGET0;
};

PS_OUT_SHADESHADOW PS_MAIN_SHADESHADOW(PS_IN_SHADESHADOW In)
{
	PS_OUT_SHADESHADOW Out = (PS_OUT_SHADESHADOW) 0.f;
	
	float fOut = 1.f;
	
	vector Diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float Bias = 0.001f;
	float ShadowIntensity = 0.6f;

	float CurrentDepth = In.vLightPosition.z / In.vLightPosition.w;
	
	float2 ShadowUV = In.vLightPosition.xy / In.vLightPosition.w;
	
	if (ShadowUV.x < -fOut || ShadowUV.x > fOut || ShadowUV.y < -fOut || ShadowUV.y > fOut || CurrentDepth < -fOut || CurrentDepth > fOut)
	{
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
	}
	else
	{
		ShadowUV.y = -ShadowUV.y;
		ShadowUV = ShadowUV * 0.5f + 0.5f;

		float shadowDepth;
		shadowDepth = g_ShadowTexture.Sample(ClampSampler, ShadowUV).r;
		
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
		if (CurrentDepth > shadowDepth + Bias)
		{
			Out.vShadeShadow.rgb *= ShadowIntensity;

			if (Diffuse.a < 0.1f)
				Out.vShadeShadow.a = Diffuse.a;
		}
		else
		{
			if (Diffuse.a < 0.1f)
				Out.vShadeShadow.a = Diffuse.a;
		}
	}
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
	float4 M		: SV_Target3;
	float4 R		: SV_Target4;
	float4 A		: SV_Target5;
	float4 E		: SV_Target6;
};

PS_OUT PS_MAIN_SHIELD(PS_IN In)
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
	
	float Metalic = mra.r;
	Out.M = float4(Metalic.xxx, 1.f);
	
	float Roughness = mra.g;
	Out.R = float4(Roughness.xxx, 1.f);

	float AO = mra.b;
	Out.A = float4(AO.xxx, 1.f);
	
	float4 color = float4(1.f, 0.8431f, 0.f, 1.f);
	float4 power = 0.8f;
	Out.E = color * power * ceo.g;
	
	return Out;	
}

technique11			DefaultTechnique
{	
	pass Weapone //------------------------------------------------------------------------------------0 Top
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* ???????????? ????????. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHIELD();
	}

	pass ShadowANIM //-----------------------------------------------------------------------------------------3 Anim ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* ???????????? ????????. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass ShadeShadow //-----------------------------------------------------------------------------------------4 Anim Shade_Shadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* ???????????? ????????. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADESHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADESHADOW();
	}
}





#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

cbuffer LightBuffer
{
	matrix g_LightView;
	matrix g_LightProj;
};

texture2D g_DiffuseTexture;
texture2D g_ShadowTexture;
texture2D g_PBRTexture;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT; 
	float3		vBiNormal : BINORMAL;
	
	float4 vRight : TEXCOORD1;
	float4 vUp  : TEXCOORD2;
	float4 vLook  : TEXCOORD3;
	float4 vTranslation  : TEXCOORD4;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

bool g_bUsingTool = false;

VS_OUT VS_MESH(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	
	if (g_bUsingTool)
	{
		matInstance = float4x4(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
	}

	vector vPosition = mul(vector(In.vPosition, 1.f), matInstance);

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vClipPos = Out.vPosition;
	return Out;
}
// VS_SHADOW_MAP
//*---------------------------------------------------------------------------------------------*

struct VS_OUT_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
	float4 vWorldPos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	if (g_bUsingTool)
	{
		matInstance = float4x4(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
	}

	vector vPosition = mul(vector(In.vPosition, 1.f), matInstance);

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vClipPos = Out.vPosition;
	return Out;
	
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
	float4 vWorldPos : TEXCOORD2;
};

VS_OUT_SHADESHADOW VS_MAIN_SHADESHADOW(VS_IN In)
{
	VS_OUT_SHADESHADOW Out = (VS_OUT_SHADESHADOW) 0.f;
	
	matrix matWV, matWVP, matLightWV, matLightWVP;

	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	matLightWV = mul(g_WorldMatrix, g_LightView);
	matLightWVP = mul(matLightWV, g_LightProj);
	
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLightPosition = mul(vector(In.vPosition, 1.f), matLightWVP);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};
struct PS_RECT_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};
struct PS_OUT
{
	vector		vColor : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	//if (Out.vColor.a < 0.2f)
	//	discard;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	return Out;
}

// SHADOW_MAP
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
	float4 vWorldPos : TEXCOORD2;
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
	
	if (color.r < 0.1f)
	{
		Alpha = color.a;
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
	float4 vWorldPos : TEXCOORD2;
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
	float ShadowIntensity = 0.3f;

	float CurrentDepth = In.vLightPosition.z / In.vLightPosition.w;
	
	float2 ShadowUV = In.vLightPosition.xy / In.vLightPosition.w;
	
	if (ShadowUV.x < -fOut || ShadowUV.x > fOut || ShadowUV.y < -fOut || ShadowUV.y > fOut || CurrentDepth < -fOut || CurrentDepth > fOut)
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
	else
	{
		ShadowUV.y = -ShadowUV.y;
		ShadowUV = ShadowUV * 0.5f + 0.5f;

		float shadowDepth, shadowAlpha;
		shadowDepth = g_ShadowTexture.Sample(DefaultSampler, ShadowUV).r;
		
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
		if (CurrentDepth > shadowDepth + Bias)
		{
			Out.vShadeShadow.rgb *= ShadowIntensity;

			if (Diffuse.a < 0.9f)
				Out.vShadeShadow.a = Diffuse.a;
		}
		else
		{
			if (Diffuse.a < 0.9f)
				Out.vShadeShadow.a = Diffuse.a;
		}
	}
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

technique11			DefaultTechnique
{
	pass DefaultShader
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MESH();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	pass Default_Frame
	{
		SetRasterizerState(CullMode_Wireframe);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MESH();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	pass ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
	pass CompareShadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SHADESHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADESHADOW();
	}
}
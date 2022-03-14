#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

cbuffer BrushDesc
{
	vector		g_vBrushPos = vector(20.f, 0.f, 10.f, 1.f);
	float		g_fRadius = 10.f;
};

cbuffer LightBuffer
{
	matrix g_LightView;
	matrix g_LightProj;
};

texture2D	g_DiffuseSourTexture;
texture2D	g_DiffuseDestTexture;
texture2D	g_FilterTexture;
texture2D	g_BrushTexture;
texture2D	g_ShadowTexture;
texture2D	g_PBRTexture;

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

float4 TerrainSplitting(float2 TexUV,float4 worldpos)
{
	float4 Diffuse;
	vector vSourDiffuse = g_DiffuseSourTexture.Sample(DefaultSampler, TexUV * 10.f);
	vector vDestDiffuse = g_DiffuseDestTexture.Sample(DefaultSampler, TexUV * 20.f);
	vector vFilterColor = g_FilterTexture.Sample(DefaultSampler, TexUV);

	vector vBrushColor = (vector) 0;

	if (g_vBrushPos.x - g_fRadius <= worldpos.x && worldpos.x < g_vBrushPos.x + g_fRadius &&
		g_vBrushPos.z - g_fRadius <= worldpos.z && worldpos.z < g_vBrushPos.z + g_fRadius)
	{
		float2 vTexUV = float2((worldpos.x - (g_vBrushPos.x - g_fRadius)) / (2.f * g_fRadius),
			((g_vBrushPos.z + g_fRadius) - worldpos.z) / (2.f * g_fRadius));
		vBrushColor = g_BrushTexture.Sample(DefaultSampler, vTexUV);
	}
	
	Diffuse = vSourDiffuse * vFilterColor.r + vDestDiffuse * (1.f - vFilterColor.r) + vBrushColor;
	//Diffuse.a = 1.f;
	
	return Diffuse;
}

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

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
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);
	
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	//Out.vPosition = mul(vector(In.vPosition, 1.f), g_LightWorld);
	//Out.vPosition = mul(Out.vPosition, g_LightView);
	//Out.vPosition = mul(Out.vPosition, g_LightProj);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vClipPos = Out.vPosition;
	
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
	//Out.vLightPosition = Out.vPosition;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vViewPos : TEXCOORD3;

};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vViewPos = mul(vector(In.vPosition, 1.f), g_ViewMatrix);
	Out.vProjPos = Out.vPosition;
	
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
	
	float4 color = g_DiffuseSourTexture.Sample(DefaultSampler, In.vTexUV);
	//TerrainSplitting(In.vTexUV, In.vWorldPos);
	
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
	
	vector Diffuse = g_DiffuseSourTexture.Sample(DefaultSampler, In.vTexUV); /*TerrainSplitting(In.vTexUV, In.vWorldPos)*/;
	
	
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
		shadowDepth = g_ShadowTexture.Sample(ClampSampler, ShadowUV).r;
		
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


struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
	float4 vViewPos : TEXCOORD3;
};

//*---------------------------------------------------------------------------------------------*
struct PS_OUT_PBR
{
	vector vMetallic : SV_TARGET0;
	vector vRoughness : SV_TARGET1;
	vector vAO : SV_TARGET2;
};

PS_OUT_PBR PS_MAIN_PBR(PS_IN In)
{
	PS_OUT_PBR Out = (PS_OUT_PBR) 0;
	
	//if (g_NonTex)
	//{
	//	Out.vMetallic = vector(1.f, 1.f, 1.f, 1.f);
	//	Out.vRoughness = vector(1.f, 1.f, 1.f, 1.f);
	//	Out.vAO = vector(1.f, 1.f, 1.f, 1.f);
	//}
	//else
	//{
	//	vector vPBRTexture = g_PBRTexture.Sample(DefaultSampler, In.vTexUV);
	
	//	Out.vMetallic = vector(vPBRTexture.rrr, 0.f);
	//	Out.vRoughness = vector(vPBRTexture.ggg, 0.f);
	//	Out.vAO = vector(vPBRTexture.bbb, 0.f);
	//}
	Out.vMetallic = vector(1.f, 1.f, 1.f, 1.f);
	Out.vRoughness = vector(1.f, 1.f, 1.f, 1.f);
	Out.vAO = vector(1.f, 1.f, 1.f, 1.f);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

struct PS_OUT
{
	vector vDiffuse : SV_TARGET0;
	vector vNormal : SV_TARGET1;
	vector vDepth : SV_TARGET2;
	vector vPosition : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	Out.vDiffuse = g_DiffuseSourTexture.Sample(DefaultSampler, In.vTexUV);
	//TerrainSplitting(In.vTexUV, In.vWorldPos);
	Out.vDiffuse.a = 1.f;
	Out.vPosition = In.vViewPos;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.0f, 0.0f);
	
	return Out;
}

technique11			DefaultTechnique
{	
	pass Default
	{
        SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}

	pass Shadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass ShadeShadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_SHADESHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADESHADOW();
	}

	pass ReadyPBR
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_PBR();
	}
}





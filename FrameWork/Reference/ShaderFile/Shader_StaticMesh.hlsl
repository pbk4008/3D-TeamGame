#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

cbuffer ShadeCheck
{
	bool g_bool = false;
	bool g_NonTex = false;
};

Texture2D g_DiffuseTexture;
Texture2D g_BiNormalTexture;
Texture2D g_ShadowTexture;
Texture2D g_MRATexture;
Texture2D g_CEOTexture;
Texture2D g_GradientTex;

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
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
};

VS_OUT VS_MAIN_STATIC(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;


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

// VS_SHADOWMAP_STATIC
//*---------------------------------------------------------------------------------------------*

struct VS_OUT_SHADOW_STATIC
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

VS_OUT_SHADOW_STATIC VS_SHADOWMAP_STATIC(VS_IN In)
{

	VS_OUT_SHADOW_STATIC Out = (VS_OUT_SHADOW_STATIC) 0;


	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vClipPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	
	return Out;
	
}
//*---------------------------------------------------------------------------------------------*

// Static Mesh ShadeShadow
//*---------------------------------------------------------------------------------------------*

struct VS_OUT_SHADESHADOW_STATIC
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vLightPosition : TEXCOORD1;
};

VS_OUT_SHADESHADOW_STATIC VS_MAIN_SHADESHADOW_STATIC(VS_IN In)
{
	VS_OUT_SHADESHADOW_STATIC Out = (VS_OUT_SHADESHADOW_STATIC) 0.f;
	
	matrix matWV, matWVP, matLightWV, matLightWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	matLightWV = mul(g_WorldMatrix, g_LightView);
	matLightWVP = mul(matLightWV, g_LightProj);
	 
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLightPosition = mul(vector(In.vPosition, 1.f), matLightWVP);
	
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
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	float4 mra : SV_Target3;
	float4 emission : SV_Target4;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	
	Out.diffuse = diffuse;
	
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = half4(1, 1, 1, 0);
	
	half Metalic = 0;
	half Roughness = 1.;
	half Ao = 1.f;

	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = Ao;
	Out.mra.a = 1.f;
	Out.emission = g_color * g_empower;

	return Out;
}

struct PS_OUT_Alpha
{
	float4 vColor : SV_TARGET0;
	float4 vWeight : SV_TARGET1;
};

PS_OUT_Alpha PS_MAIN_Alpha(PS_IN In)
{
	PS_OUT_Alpha Out = (PS_OUT_Alpha) 0;
	
	//half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);

	Out.vColor.rgb = g_color;
	
	half4 weight = half4(g_Weight.xxx, 1.f);
	
	Out.vWeight = weight;

	return Out;
}
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

struct PS_OUT_TOOL
{
	vector vDiffuse : SV_TARGET0;
	vector vNormal : SV_TARGET1;
	vector vDepth : SV_TARGET2;
	
};

PS_OUT_TOOL PS_MAIN_TOOL(PS_IN In)
{
	PS_OUT_TOOL Out = (PS_OUT_TOOL) 0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	Out.vDiffuse.a = 1.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 150.0f, 0.0f, 0.0f);

	return Out;
}

//*---------------------------------------------------------------------------------------------*
// VS Motion Trail
struct VS_OUT_MOTIONTRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vUvDepth : TEXCOORD0;
};

VS_OUT_MOTIONTRAIL VS_MAIN_MOTIONTRAIL(VS_IN In)
{
	VS_OUT_MOTIONTRAIL Out = (VS_OUT_MOTIONTRAIL) 0;
	
	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;

	return Out;
}
// PS Motion Trail
struct PS_IN_MOTIONTRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT_MOTIONTRAIL
{
	half4 Motiontrail : SV_TARGET0;
};

PS_OUT_MOTIONTRAIL PS_MAIN_MOTIONTRAIL(PS_IN_MOTIONTRAIL In)
{
	PS_OUT_MOTIONTRAIL Out = (PS_OUT_MOTIONTRAIL) 0;

	half3 color = g_GradientTex.Sample(DefaultSampler, half2(g_UVdvid, 0)).rgb;
	half4 normal = half4(In.vNormal.xyz, 0.f);
	Out.Motiontrail = MotionTrailRim(normal, g_camdir, g_rimintensity, color);
	Out.Motiontrail.a *= g_Fade;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

technique11 DefaultTechnique
{
	pass StaticMesh //------------------------------------------------------------------------------------0 StaticMeshRender
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TOOL();
	}
	pass StaticMeshWire //------------------------------------------------------------------------------------1 StaticMeshRenderWireFrame
	{
		SetRasterizerState(CullMode_Wireframe);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TOOL();
	}
	pass StaticMeshClient //------------------------------------------------------------------------------------2 StaticMeshRenderClient
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass ShadowStaticMesh //------------------------------------------------------------------------------------3 Static ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_SHADOWMAP_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass Motiontrail //------------------------------------------------------------------------------------4 motiontrail
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_MOTIONTRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MOTIONTRAIL();
	}

	pass StaticMeshAlpha //------------------------------------------------------------------------------------5 Alpha
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Alpha();
	}
}





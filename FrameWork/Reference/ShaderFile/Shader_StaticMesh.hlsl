#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix g_WorldMatrix = (matrix) 0;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
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

cbuffer ShadeCheck
{
	bool g_bool = false;
};

texture2D g_DiffuseTexture;
texture2D g_ShadowTexture;
texture2D g_BiNormalTexture;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};


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
	float2 vTexUV : TEXCOORD0;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vProjPos : TEXCOORD1;
	float4 vViewPos : TEXCOORD2;
};

VS_OUT VS_MAIN_STATIC(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;


	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	
	Out.vProjPos = Out.vPosition;
	Out.vViewPos = mul(vector(In.vPosition, 1.f), matWV);

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
	float2 vTexUV : TEXCOORD0;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vProjPos : TEXCOORD1;
	float4 vViewPos : TEXCOORD2;
};

struct PS_OUT
{
	vector vDiffuse : SV_TARGET0;
	vector vNormal : SV_TARGET1;
	vector vDepth : SV_TARGET2;
	vector vPosition : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	Out.vPosition = In.vViewPos;
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = 1.f;

	float3 vNormalMap = g_BiNormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	float3 vNormal = normalize(vNormalMap.xyz * 2.f - 1.f);
	vector pos = (vector) 1;
	float4x4 matTangent = float4x4(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal), pos);
	
	matTangent = transpose(matTangent);
	/* -1 ~ 1 */
	/* 0 ~ 1 */
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	if (g_bool == true)
		Out.vNormal = mul(vector(vNormal.xyz, 0.f), matTangent);
	else
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.0f, 0.0f);


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

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = 1.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.0f, 0.0f);

	return Out;
}

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

	pass ShadeShadowStaticMesh //------------------------------------------------------------------------------------4 Static Shade_Shadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADESHADOW_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADESHADOW();
	}
	pass StaticMeshCull //------------------------------------------------------------------------------------5 StaticMeshRenderBackOn
	{
		SetRasterizerState(CullMode_BackOn);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TOOL();
	}
	pass StaticMeshCullWire //------------------------------------------------------------------------------------5 StaticMeshRenderBackOnWire
	{
		SetRasterizerState(CullMode_BackOnWireFrame);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_STATIC();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TOOL();
	}
}





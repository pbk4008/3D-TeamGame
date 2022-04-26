#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

// Textures
texture2D	g_DiffuseTexture;
texture2D	g_NormalTex;
texture2D	g_MaskTex;
texture2D	g_NoiseTex;
// Time
float g_fLifeTime;
float g_fAccTime;
// Alpha
float g_fAlpha;
// UV
bool g_isReverse;
float2 g_vPlusUV;
float2 g_vTiling;
// X
bool g_isFlowX;
float g_fFlowSpeedX;
// Y
bool g_isFlowY;
float g_fFlowSpeedY;
// Color
bool g_isCustomColor;
float3 g_vColor;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_FLOW(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	if (g_isFlowX)
	{
		Out.vTexUV.x += g_vPlusUV.x;
	}
	if (g_isFlowY)
	{
		Out.vTexUV.y += g_vPlusUV.y;
	}
	if (g_isReverse)
	{
		float2 TexUV = { Out.vTexUV.y, Out.vTexUV.x };
		Out.vTexUV = TexUV;
	}

	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x * g_vTiling.x, In.vTexUV.y * g_vTiling.y));
	if (g_isCustomColor)
	{
		Out.vDiffuse.xyz *= g_vColor.xyz;
	}
	Out.vDiffuse.a *= g_fAlpha;

	/* -1 ~ 1 */
	/* 0 ~ 1 */
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.0f, 0.0f);

	return Out;
}

PS_OUT PS_MAIN_Border(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(BorderSampler, float2(In.vTexUV.x * g_vTiling.x, In.vTexUV.y * g_vTiling.y));
	if (g_isCustomColor)
	{
		Out.vDiffuse.xyz *= g_vColor.xyz;
	}
	Out.vDiffuse.a *= g_fAlpha;

	/* -1 ~ 1 */
	/* 0 ~ 1 */
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.0f, 0.0f);

	return Out;
}

struct PS_OUT_Alpha
{
	vector		Color : SV_TARGET0;
	vector		Weight : SV_TARGET1;
};

PS_OUT_Alpha PS_MAIN_Alpha (PS_IN In)
{
	PS_OUT_Alpha		Out = (PS_OUT_Alpha)0;

	Out.Color = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x * g_vTiling.x, In.vTexUV.y * g_vTiling.y));
	if (g_isCustomColor)
	{
		Out.Color.rgb *= g_vColor.rgb;
	}
	Out.Color.a *= g_fAlpha;
	
	Out.Weight = g_Weight;

	return Out;
}

struct PS_OUT_AlphaNoBloom
{
	vector		Color : SV_TARGET0;
};

PS_OUT_AlphaNoBloom PS_MAIN_AlphaNoBloom(PS_IN In)
{
	PS_OUT_AlphaNoBloom		Out = (PS_OUT_AlphaNoBloom)0;

	Out.Color = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x * g_vTiling.x, In.vTexUV.y * g_vTiling.y));
	if (g_isCustomColor)
	{
		Out.Color.xyz *= g_vColor.xyz;
	}
	Out.Color.a *= g_fAlpha;

	
	return Out;
}

struct PS_OUT_NONALPHA
{
	half4 diffuse	: SV_TARGET0;
	half4 normal	: SV_TARGET1;
	half4 depth		: SV_TARGET2;
	half4 mra		: SV_Target3;
	half4 emission	: SV_Target4;
};

PS_OUT_NONALPHA PS_MAIN_NonAlpha(PS_IN In)
{
	PS_OUT_NONALPHA Out = (PS_OUT_NONALPHA)0;

	Out.diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.depth = half4(In.vPosition.z / In.vPosition.w, In.vPosition.w / 300.f, 0.f, 0.f);
	Out.normal = half4(1, 1, 1, 0);

	Out.mra.r = 0.f;
	Out.mra.g = 1.f;
	Out.mra.b = 1.f;
	Out.mra.a = 1.f;

	Out.emission = half4(g_vColor,1.f) * g_empower;

	//if (0.1f >= Out.diffuse.a)
	//	discard;

	return Out;
}

technique11			DefaultTechnique
{
	// 0
	pass Default
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 1
	pass None_ZBufferDisable_AlphaAdditive
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 2
	pass CCW_ZBufferDisable_AlphaAdditive
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 3
	pass CW_ZBufferDisable_AlphaAdditive
	{
		SetRasterizerState(CullMode_Reverse);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 4
	pass None_ZDefault_AlphaAdditive
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 5
	pass None_ZBufferDisable_AlphaAdditive_Border
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_Border();
	}
	// 6
	pass AlphaGroup
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_Alpha();
	}
	// 7
	pass AlphaNoBloomGroup
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZBufferDisable, 0);
		/*SetBlendState(AlphaBlending2, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);*/

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_AlphaNoBloom();
	}
	// 8 NonAlpha emesive  ex) laser
	pass test
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_NonAlpha();
	}
	// 9
	pass NoneAlpha
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_FLOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
}

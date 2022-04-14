#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D g_DiffuseTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
};


 VS_OUT VS_MAIN(VS_IN In)
{
	 VS_OUT			Out = (VS_OUT)0;

	 matrix			matWV, matWVP;
	 matWV = mul(g_WorldMatrix, g_ViewMatrix);
	 matWVP = mul(matWV, g_ProjMatrix);

	 Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	 Out.vTexUV = In.vTexUV;

	 return Out;
}

 VS_OUT_TRAIL VS_MAIN_TRAIL(VS_IN In)
 {
	 VS_OUT_TRAIL Out = (VS_OUT_TRAIL)0;

	 // VS_IN In : 정점버퍼에 정의되어있던 정점정보를 그대로 받아온것이다. 
	 matrix matWV, matWVP;

	 matWV = mul(g_WorldMatrix, g_ViewMatrix);
	 matWVP = mul(matWV, g_ProjMatrix);

	 Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	 Out.vUvDepth.xy = In.vTexUV.xy;
	 Out.vUvDepth.zw = Out.vPosition.zw;

	 return Out;
 }

 struct PS_IN
 {
	 float4		vPosition : SV_POSITION;
	 float2		vTexUV : TEXCOORD0;
 };

 struct PS_OUT
 {
	 vector		vColor : SV_TARGET0;
 };

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a <= 0.1)
		discard;
	
	return Out;
}

struct PS_IN_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT_TRAIL
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	half4 mra : SV_Target3;
	half4 emission : SV_Target4;
};

PS_OUT_TRAIL PS_MAIN_TRAIL(PS_IN_TRAIL In)
{
	PS_OUT_TRAIL Out = (PS_OUT_TRAIL)0;

	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);

	Out.diffuse = diffuse;

	if (Out.diffuse.r <= 0.f || Out.diffuse.g <= 0.f || Out.diffuse.b <= 0.f)
		discard;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1, 0);

	Out.mra.r = 0;
	Out.mra.g = 1;
	Out.mra.b = 1;
	Out.mra.a = 1;

	Out.emission = diffuse * g_empower;

	return Out;
}

technique11 DefaultTechnique
{
	pass Normal
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	pass TrailRect
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass AlphaBlend
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass NonAlpha
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}
}

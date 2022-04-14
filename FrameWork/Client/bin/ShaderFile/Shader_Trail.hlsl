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
}

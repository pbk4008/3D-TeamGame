#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

cbuffer Noisebuffer
{
	float	g_frametime;
	float3	g_scrollspeeds;
	float3	g_scales;
};

cbuffer Distortionbuffer
{
	float2 g_distortion1;
	float2 g_distortion2;
	float2 g_distortion3;
	float g_distortionscale;
	float g_distortionbias;
	float g_deltatime;
};

Texture2D g_DiffuseTexture;
Texture2D g_DistortionTex;
Texture2D g_DistorionMaskTex;
Texture2D g_DissolveTex;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;

};

 VS_OUT_TRAIL VS_MAIN_DISTORTION(VS_IN In)
{
	VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vUvDepth.xy = In.vTexUV.xy;	
	Out.vUvDepth.zw = Out.vPosition.xy;
	
	return Out;
}

struct PS_IN_DISTORTION
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;

};

struct PS_OUT_DISTORTION
{
	float4 diffuse : SV_TARGET0;
};

PS_OUT_DISTORTION PS_MAIN_TRAIL(PS_IN_DISTORTION In)
{
	PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;
	
	half2 uv = In.vUvDepth.xy;
	
	half3 nois = g_DistorionMaskTex.Sample(DefaultSampler, half2(uv.y * 2.0f, uv.x * 0.1f + g_deltatime * 0.125f)).rgb / 8.0;
	half2 realuv = (uv.xy - nois.xy);
	half4 color = g_DistorionMaskTex.Sample(DefaultSampler, realuv);
	
	Out.diffuse = color;
	
	return Out;
}

PS_OUT_DISTORTION PS_MAIN_WALL(PS_IN_DISTORTION In)
{
	PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;
	
	Out.diffuse = Noisfunction(g_DiffuseTexture, DefaultSampler, In.vUvDepth.xy, g_deltatime, g_color);

	if (g_bdissolve == true)
	{
		Dissolve(g_DissolveTex, DefaultSampler, In.vUvDepth.xy, g_dissolvetime);
	}
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Trail
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}

	pass Wall
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WALL();
	}
}

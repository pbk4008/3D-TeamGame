#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix  = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

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

cbuffer Noisebuffer
{
	float	g_frametime;
	float3	g_scrollspeeds;
	float3	g_scales;
	float	g_Weight;
};

cbuffer Distortionbuffer
{
	float2 g_distortion1;
	float2 g_distortion2;
	float2 g_distortion3;
	float g_distortionscale;
	float g_distortionbias;
};

Texture2D g_DiffuseTexture;
Texture2D g_DistortionTex;
Texture2D g_DistorionMaskTex;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
	float2 texcoord3 : TEXCOORD3;
};

 VS_OUT_TRAIL VS_MAIN_TRAIL(VS_IN In)
{
	//VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

	//matrix matWV, matWVP;

	//matWV = mul(g_WorldMatrix, g_ViewMatrix);
	//matWVP = mul(matWV, g_ProjMatrix);
		
	//Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	//Out.vUvDepth.xy = In.vTexUV.xy;
	//Out.vUvDepth.zw = Out.vPosition.zw;
	VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vUvDepth.xy = In.vTexUV.xy;

	Out.texcoord1 = (In.vTexUV * g_scales.x);
	Out.texcoord1.y = Out.texcoord1.y + (g_frametime * g_scrollspeeds.x);
	
	Out.texcoord2 = (In.vTexUV * g_scales.y);
	Out.texcoord2.y = Out.texcoord2.y + (g_frametime * g_scrollspeeds.y);
	
	Out.texcoord3 = (In.vTexUV * g_scales.z);
	Out.texcoord3.y = Out.texcoord3.y + (g_frametime * g_scrollspeeds.z);
	
	Out.vUvDepth.zw = Out.vPosition.xy;
	
	return Out;
}

struct PS_IN_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
	float2 texcoord3 : TEXCOORD3;
};

struct PS_OUT_TRAIL
{
	float4 diffuse : SV_TARGET0;
	float4 weight : SV_Target1;
};

PS_OUT_TRAIL PS_MAIN_TRAIL(PS_IN_TRAIL In)
{
	PS_OUT_TRAIL Out = (PS_OUT_TRAIL) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 distortion = g_DistortionTex.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 distortionmask = g_DistorionMaskTex.Sample(DefaultSampler, In.vUvDepth.xy);
	
	//clip(distortionmask.r - 0.1f);
	
	//float2 distortUV = In.vUvDepth.xy + (distortion.x * 0.02f * distortionmask.x);
	//distortUV.y = clamp(distortUV.y, 0, 1);
	
	//float4 diffuse2 = g_DiffuseTexture.Sample(DefaultSampler, distortUV);
	//diffuse2 = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	//Out.diffuse = diffuse2;
	
	
	half4 noise1, noise2, noise3, finalnoise, firecolor, alphacolor;
	half perturb;
	half4 noisecoords;
	
	noise1 = g_DistorionMaskTex.Sample(DefaultSampler, In.texcoord1);
	noise2 = g_DistorionMaskTex.Sample(DefaultSampler, In.texcoord2);
	noise3 = g_DistorionMaskTex.Sample(DefaultSampler, In.texcoord3);
	
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;
	
	noise1.xy = noise1.xy * g_distortion1.xy;
	noise2.xy = noise2.xy * g_distortion2.xy;
	noise3.xy = noise3.xy * g_distortion3.xy;
	
	finalnoise = noise1 + noise2 + noise3;

	perturb = ((1.0f - In.vUvDepth.y) * g_distortionscale) + g_distortionbias;
	
	noisecoords.xy = (finalnoise.xy * perturb) + In.vUvDepth.xy;
	
	firecolor = g_DiffuseTexture.Sample(ClampSampler, noisecoords.xy);
	alphacolor = g_DistortionTex.Sample(ClampSampler, noisecoords.xy);
	
	firecolor.a = alphacolor;
	
	Out.diffuse = firecolor;
	Out.weight = float4(g_Weight.xxx, 1.f);
	//Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	//Out.normal = float4(1, 1, 1, 0);
	
	//Out.M = float4(0, 0, 0, 1);
	//Out.R = float4(1, 1, 1, 1);
	//Out.A = float4(1, 1, 1, 1);
	//float4 color = float4(0.99f, 0.43f, 0.2f, 0.3f);
	//float4 power = 0.1f;
	//Out.E = color * power;

	return Out;
}



technique11 DefaultTechnique
{
	pass TrailRect
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}
	pass TrailBlend
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}
}

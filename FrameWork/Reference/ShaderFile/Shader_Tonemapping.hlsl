#include "Shader_RenderState.hpp"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	//AddressU = clamp;
	//AddressV = clamp;
};

cbuffer ConstBuffer
{
	float Exposure;
	float GaussianScalar;
	float rcp_bloom_tex_w;
	float rcp_bloom_tex_h;
};

texture2D g_HDRDiffuseTexture;
texture2D g_HDRSpecularTexture;

texture2D g_LuminanceTexture;
texture2D g_BloomTexture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;
	
	Out.vPosition = vector(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT_TONE
{
	vector vHDRDiffuse : SV_Target0;
	vector vHDRSpecualr : SV_Target1;
};

PS_OUT_TONE PS_MAIN_TONEMAPPING(PS_IN In)
{
	PS_OUT_TONE Out = (PS_OUT_TONE) 0;
	
	float Gamma = 2.2f;
	vector luminanceBase = g_LuminanceTexture.Sample(DefaultSampler, float2(0.5f, 0.5f));
	
	
	vector vTotalcolor = g_HDRDiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	// dx ±âº» Åæ¸ÅÇÎ
	float Luminance = 0.4f;
	//float Luminance = 0.08f;
	float MiddleGray = 0.18f;
	float WhiteCutoff = 0.9f;
	
	vector color;
	color.rgb = vTotalcolor.rgb * MiddleGray / (Luminance + 0.001f);
	
	color.rgb *= (1.f + (color.rgb / (WhiteCutoff * WhiteCutoff)));
	color.rgb /= (1.f + color.rgb);
	color.a = 1.f;
	
	Out.vHDRDiffuse = color;
	
	
	vector vSpeccular = g_HDRSpecularTexture.Sample(DefaultSampler, In.vTexUV);
	
	Luminance = 0.1f;
	//Luminance = 0.08f;
	MiddleGray = 0.18f;
	WhiteCutoff = 0.9f;
	
	vector vSpecColor;
	vSpecColor.rgb = vSpeccular.rgb * MiddleGray / (Luminance + 0.001f);
	
	vSpecColor.rgb *= (1.f + (vSpecColor.rgb / (WhiteCutoff * WhiteCutoff)));
	vSpecColor.rgb /= (1.f + vSpecColor.rgb);
	vSpecColor.a = 1.f;
	
	Out.vHDRSpecualr = vSpecColor;
	
	float xWeight = frac(In.vTexUV.x / rcp_bloom_tex_w) - 0.5f;
	float xDir = xWeight;
	xWeight = abs(xWeight);
	xDir /= xWeight;
	xDir *= rcp_bloom_tex_w;
	
	float yWeight = frac(In.vTexUV.y / rcp_bloom_tex_h) - 0.5f;
	float yDir = yWeight;
	yWeight = abs(yWeight);
	yDir /= yWeight;
	yDir *= rcp_bloom_tex_h;
	
	float4 BaseBloom = ((1.0f - xWeight) * (1.0f - yWeight)) * g_BloomTexture.Sample(DefaultSampler, In.vTexUV);
	BaseBloom += (xWeight * (1.f - yWeight)) * g_BloomTexture.Sample(DefaultSampler, In.vTexUV + float2(xDir, 0.f));
	BaseBloom += (yWeight * (1.f - xWeight)) * g_BloomTexture.Sample(DefaultSampler, In.vTexUV + float2(0.f, yDir));
	BaseBloom += (xWeight * yWeight) * g_BloomTexture.Sample(DefaultSampler, In.vTexUV + float2(xDir, yDir));
	
	vector finaldiffuse = 0.25f * BaseBloom;
	
	float Lp = (Exposure / luminanceBase.r) * max(finaldiffuse.r ,max(finaldiffuse.g, finaldiffuse.b));
	
	float LmSqr = (luminanceBase.g + GaussianScalar * luminanceBase.g) * (luminanceBase.g + GaussianScalar * luminanceBase.g);
	
	float toneScalar = (Lp * (1.f + (Lp / (LmSqr)))) / (1.f + Lp);
	
	finaldiffuse = finaldiffuse * toneScalar;
	finaldiffuse.a = 1.f;
	
	Out.vHDRDiffuse += finaldiffuse; /*pow(abs(finaldiffuse), 1.f / Gamma)*/
	
	return Out;
}

technique11 Luminance
{
	pass ToneMapping
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZWriteDisable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//
		//AlphaBlending
		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TONEMAPPING();
	}
}
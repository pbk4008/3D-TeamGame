#include "Shader_Calculate.hpp"

sampler DefaultSampler = sampler_state
{
	filter = anisotropic;
	MaxAnisotropy = 16;

	AddressU = mirror;
	AddressV = mirror;
};

sampler radialsampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer RtPixel
{
	float2	g_RtperPixel;
};

cbuffer ConstBuffer
{
	vector g_BrightPassOffset[4];
	float g_BrightPassThreshold;
	float g_Weight;
};

Texture2D g_Basetexture;

// bloom
Texture2D g_BaseBlur2Texture;
Texture2D g_BaseBlur4Texture;
Texture2D g_BaseBlur8Texture;
Texture2D g_BaseBlur16Texture;


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

struct PS_OUT
{
	vector vOutColor : SV_Target0;
};

PS_OUT PS_MAIN_VerticalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 color = 0;
	for (int i = 0; i < 13; ++i)
	{
		color += g_Basetexture.Sample(DefaultSampler, float2(
													In.vTexUV.x + g_RtperPixel.x,
													In.vTexUV.y + (pixelKernel[i] * g_RtperPixel.y * g_blurSize) + g_RtperPixel.y))
													* blurWeights[i];
	}
	
	Out.vOutColor = color;
	
	return Out;
}

PS_OUT PS_MAIN_HorizontalBlur(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 color = 0;
	for (int i = 0; i < 13; ++i)
	{
		color += g_Basetexture.Sample(DefaultSampler, float2(
													In.vTexUV.x + (pixelKernel[i] * g_RtperPixel.x * g_blurSize) + g_RtperPixel.x,
													In.vTexUV.y + g_RtperPixel.y))
													* blurWeights[i];
	}
	
	Out.vOutColor.xyz = color.xyz;
	
	return Out;
}

PS_OUT PS_MAIN_BrightPass(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	float4 Average = float4(0.f, 0.f, 0.f, 0.f);
	
	for (int i = 0; i < 4; i++)
	{
		Average += g_Basetexture.Sample(DefaultSampler, (In.vTexUV + float2(g_BrightPassOffset[i].x, g_BrightPassOffset[i].y)));
	}
	Average *= 0.25f;
	
	//float Luminance = max(Average.r, max(Average.g, Average.b));
	float Luminance = 0.5f * (max(Average.r, max(Average.g, Average.b)) + min(Average.r, min(Average.g, Average.b)));
	
	if (Luminance > g_BrightPassThreshold)
		Average = float4(0.f, 0.f, 0.f, 1.f);
		
	Out.vOutColor = Average;
	
	return Out;
}

PS_OUT PS_MAIN_Bloom(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	
	float4 base2 = g_BaseBlur2Texture.Sample(DefaultSampler, In.vTexUV);
	float4 base4 = g_BaseBlur4Texture.Sample(DefaultSampler, In.vTexUV);
	float4 base8 = g_BaseBlur8Texture.Sample(DefaultSampler, In.vTexUV);
	float4 base16 = g_BaseBlur16Texture.Sample(DefaultSampler, In.vTexUV);
	
	float4 baseBloom = (base2 * g_Weight) + (base4 * g_Weight) + (base8 * g_Weight) + (base16 * g_Weight);
	
	Out.vOutColor = baseBloom;
	
	return Out;
}

PS_OUT PS_MAIN_Bloom2(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 base = g_Basetexture.Sample(DefaultSampler, In.vTexUV);
	float4 base2 = g_BaseBlur2Texture.Sample(DefaultSampler, In.vTexUV);
	float4 base4 = g_BaseBlur4Texture.Sample(DefaultSampler, In.vTexUV);
	
	float4 baseBloom = (base * g_Weight) + (base2 * g_Weight) + (base4 * g_Weight);
	
	Out.vOutColor = baseBloom;
	
	return Out;
}

PS_OUT PS_MAIN_RADIAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 maintex = g_Basetexture.Sample(radialsampler, In.vTexUV);
	
	float2 perpixel = float2(1.0f / 1280.0f, 1.0f / 720.f);
	int samples = 64;
	float2 center = float2(0.5, 0.5);
	float blurstart = 0.8;
	float blurwidth = 0.1;
	float2 uv = In.vTexUV;
		
	uv -= center;
	float precompute = blurwidth * (1.0 / float(samples - 1));
		
	float4 color = float4(0, 0, 0, 0);
		
	for (int i = 0; i < samples; i++)
	{
		float scale = blurstart + (float(i) * precompute);
		color += g_Basetexture.Sample(DefaultSampler, uv * scale + center);
	}
		
	color /= float(samples);
	Out.vOutColor = color;
	
	if (Out.vOutColor.r <= 0 && Out.vOutColor.g <= 0 && Out.vOutColor.b <= 0)
		discard;
		
	return Out;
}

//--------------------------------------------------------------------------------------------------------------------------//
technique11 PostProcess
{
	pass VerticalBlur
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VerticalBlur();
	}

	pass HorizontalBlur
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HorizontalBlur();
	}
	
	pass BrightPass
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BrightPass();
	}

	pass Bloom
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Bloom();
	}

	pass Bloom2
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Bloom2();
	}

	pass Radial
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RADIAL();
	}
}
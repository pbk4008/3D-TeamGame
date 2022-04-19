#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix	g_WorldMatrix = (matrix)0;
	matrix	g_ViewMatrix = (matrix) 0;
	matrix	g_ProjMatrix = (matrix) 0;
};

cbuffer Noisebuffer
{
	float	g_frametime;
	float3	g_scrollspeeds;
	float3	g_scales;
};

cbuffer Distortionbuffer
{
	float2	g_distortion1;
	float2	g_distortion2;
	float2	g_distortion3;
	float	g_distortionscale;
	float	g_distortionbias;
};

texture2D g_FireTexture;
texture2D g_NoiseTexture;
texture2D g_AlphaTexture;

sampler WrapSampler = sampler_state
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

struct VS_IN
{
	float3		vPosition		:		POSITION;
	float2		vTexUV			:		TEXCOORD0;

	float4		vRight			:		TEXCOORD1;
	float4		vUp				:		TEXCOORD2;
	float4		vLook			:		TEXCOORD3;
	float4		vTranslation	:		TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition	: SV_POSITION;
	float2		vTexUV		: TEXCOORD0;	
	float2		texcoord1	: TEXCOORD1;
	float2		texcoord2	: TEXCOORD2;
	float2		texcoord3	: TEXCOORD3;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;	

	matrix	matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector	vPosition = mul(vector(In.vPosition, 1.f), matInstance);

	matrix			matWV, matWVP;	

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;	

	Out.texcoord1 = (In.vTexUV * g_scales.x);
	Out.texcoord1.y = Out.texcoord1.y + (g_frametime * g_scrollspeeds.x);
	
	Out.texcoord2 = (In.vTexUV * g_scales.y);
	Out.texcoord2.y = Out.texcoord1.y + (g_frametime * g_scrollspeeds.y);
	
	Out.texcoord3 = (In.vTexUV * g_scales.z);
	Out.texcoord3.y = Out.texcoord1.y + (g_frametime * g_scrollspeeds.z);
	
	return Out;

}


struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexUV		: TEXCOORD0;
	float2 texcoord1	: TEXCOORD1;
	float2 texcoord2	: TEXCOORD2;
	float2 texcoord3	: TEXCOORD3;
};

struct PS_OUT
{
	float4	vColor	: SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	float4 noise1, noise2, noise3, finalnoise, firecolor, alphacolor;
	float perturb;
	float2 noisecoords;
	
	noise1 = g_NoiseTexture.Sample(WrapSampler, In.texcoord1);
	noise2 = g_NoiseTexture.Sample(WrapSampler, In.texcoord2);
	noise3 = g_NoiseTexture.Sample(WrapSampler, In.texcoord3);
	
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;
	
	noise1.xy = noise1.xy * g_distortion1.xy;
	noise2.xy = noise2.xy * g_distortion2.xy;
	noise3.xy = noise3.xy * g_distortion3.xy;
	
	finalnoise = noise1 + noise2 + noise3;

	perturb = ((1.0f - In.vTexUV.y) * g_distortionscale) + g_distortionbias;
	
	noisecoords.xy = (finalnoise.xy * perturb) + In.vTexUV.xy;
	
	firecolor = g_FireTexture.Sample(ClampSampler, noisecoords.xy);
	alphacolor = g_AlphaTexture.Sample(ClampSampler, noisecoords.xy);
	
	firecolor.a = alphacolor.a;
	
	Out.vColor = firecolor;
	
	return Out;	
}

struct PS_OUT_TEST
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	float4 M : SV_Target3;
	float4 R : SV_Target4;
	float4 A : SV_Target5;
	float4 E : SV_Target6;
};

PS_OUT_TEST PS_MAIN_TEST(PS_IN In)
{
	PS_OUT_TEST Out = (PS_OUT_TEST) 0;

	float4 noise1, noise2, noise3, finalnoise, firecolor, alphacolor;
	float perturb;
	float2 noisecoords;
	
	noise1 = g_NoiseTexture.Sample(WrapSampler, In.texcoord1);
	noise2 = g_NoiseTexture.Sample(WrapSampler, In.texcoord2);
	noise3 = g_NoiseTexture.Sample(WrapSampler, In.texcoord3);
	
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;
	
	noise1.xy = noise1.xy * g_distortion1.xy;
	noise2.xy = noise2.xy * g_distortion2.xy;
	noise3.xy = noise3.xy * g_distortion3.xy;
	
	finalnoise = noise1 + noise2 + noise3;

	perturb = ((1.0f - In.vTexUV.y) * g_distortionscale) + g_distortionbias;
	
	noisecoords.xy = (finalnoise.xy * perturb) + In.vTexUV.xy;
	
	firecolor = g_FireTexture.Sample(ClampSampler, noisecoords.xy);
	alphacolor = g_AlphaTexture.Sample(ClampSampler, noisecoords.xy);
	
	firecolor.a = alphacolor.a;
	
	Out.diffuse = firecolor;

	Out.depth = float4(In.vPosition.z / In.vPosition.w, In.vPosition.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1, 0);

	Out.M = float4(0, 0, 0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
	float4 color = float4(1.f, 0.6f, 0.3f, 1.f);
	float4 power = 0.4f;
	Out.E = /*color * power * diffuse*/float4(0,0,0,1);

	if (Out.diffuse.a < 0.1f)
		discard;
	
	return Out;
}



technique11			DefaultTechnique
{
	pass Fire
	{
		//SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	pass Test
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST();
	}
}





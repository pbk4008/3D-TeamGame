#include "Shader_Calculate.hpp"
#include "Shader_LightUtil.hlsli"
#include "Shader_DitherUtil.hlsli"
#include "Shader_ShadowUtil.hlsli"

#pragma pack_matrix(row_major);

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler SkyBoxSampler = sampler_state
{
	AddressU = mirror;
	AddressV = mirror;
};

sampler point_clamp_sampler = sampler_state
{	
	filter = min_mag_mip_point;

	AddressU = clamp;
	AddressV = clamp;
};

sampler linear_clamp_sampler = sampler_state
{
	filter = min_mag_mip_linear;

	AddressU = clamp;
	AddressV = clamp;
};
SamplerComparisonState shadowsampler = sampler_state
{
	filter = min_mag_mip_linear;

	AddressU = border;
	AddressV = border;
};

//SamplerComparisonState shadow_sampler = shadowsampler;

cbuffer ShaderCheck
{
	bool g_bPBRHDR;
	bool g_bHDR;
	bool g_shadow;
	bool g_outline;
};

cbuffer LightDesc
{
	vector g_vLightDir;
	vector g_vLightPos;
	float g_fRange;
	vector g_vLightDiffuse;
	vector g_vLightAmbient;
	vector g_vLightSpecular;
	matrix g_LightViewProj;
};

cbuffer MaterialDesc
{
	vector g_vMtrlAmbient = (vector) 1.f;
	vector g_vMtrlDiffuse = (vector) 1.f;
	vector g_vMtrlSpecular = (vector) 1.f;
};

cbuffer CameraDesc
{
	vector g_vCamPosition;
};

cbuffer MatrixInverse
{
	matrix g_MainCamProjMatrix;
	matrix g_ProjMatrixInv;
	matrix g_ViewMatrixInv;
	matrix g_shadowmatrix;
};

texture2D g_SkyBoxTexutre;

// Lighting
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_DepthTexture;

Texture2D g_Metallic;
Texture2D g_Roughness;
Texture2D g_AO;

texture2D g_SSS;

// belnding
Texture2D g_ShadeTexture;
Texture2D g_SpecularTexture;

Texture2D g_ShadowMapTex;
Texture2D g_ShadowTexture;

Texture2D g_AlphaTexture;

Texture2D<float> depthTex;

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
struct VS_OUT_Qude
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

VS_OUT_Qude VS_MAIN_Qude(VS_IN In)
{
	VS_OUT_Qude Out = (VS_OUT_Qude) 0;
	
	Out.vPosition = float4(2 * (In.vTexUV.x - 0.5f), -2 * (In.vTexUV.y - 0.5f), 0.0f, 1.f);

	Out.vTexUV = In.vTexUV;

	return Out;
}
/* SV_POSITION을 가진 데잍처에대해서만 원근투영.(정점의 w값으로 xyzw를 나눈다.) */
/* 뷰포트로 변환한다. */
/* 래스터라이즈.(둘러쌓여진 정점의 정보를 바탕으로 하여. 픽셀정보를 생성한다.) */

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};


/* 1. 픽셀의 색을 결정한다. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	return Out;
}

struct PS_OUT_LIGHTACC
{
	vector vShade : SV_TARGET0;
	vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHTACC PS_MAIN_LIGHTACC_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHTACC Out = (PS_OUT_LIGHTACC) 0;
	
	float2 uvRT = In.vTexUV;

	vector vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, uvRT);
	vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, uvRT);
	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, uvRT);
	float Metallic = g_Metallic.Sample(DefaultSampler, uvRT).r;
	float Roughness = g_Roughness.Sample(DefaultSampler, uvRT).r;
	float AO = g_AO.Sample(DefaultSampler, uvRT).r;
	float fViewZ = vDepthDesc.y * 300.f;
	
	float3 normaltest = vNormalDesc.xyz;
	if (!any(normaltest))
		clip(0);

	vector vWorldPos;
	vWorldPos.x = (uvRT.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (uvRT.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.x * fViewZ;
	vWorldPos.w = fViewZ;
	
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
	vector normal = vector(normaltest * 2.f - 1.f, 0.f);
	float lightpow = 2.f;
	
	//Out.vShade 
	
	if (g_bPBRHDR == true)
	{
		float3 normal3 = normalize(normal.xyz);
		float3 N = normal3;
		float3 V = normalize(g_vCamPosition.xyz - vWorldPos.xyz);
		//float3 L = normalize(g_vLightPos.xyz - vWorldPos.xyz);
		float3 L = g_vLightDir.xyz * -1;
		float F0 = 0.93;
		
		float alpha = Roughness * Roughness;
		
		float3 H = normalize(V + L);
		
		float NdotL = saturate(dot(N, L));
		float NdotV = saturate(dot(N, V));
		float NdotH = saturate(dot(N, H));
		float LdotH = saturate(dot(L, H));
		
		float _F;
		float _D;
		float _V;
		
		float alphaSqr = alpha * alpha;
		float denom = NdotH * NdotH * (alphaSqr - 1.0f) + 1.0f;
		_D = alphaSqr / (PI * denom * denom);
		
		float LdotH5 = pow(1.0f - LdotH, 5);
		_F = F0 + (1.0f - F0) * LdotH5;
		
		float k = alpha / 2.0f;
		_V = (1.0f / (NdotL * (1.0f - k) + k)) * (1.0f / (NdotV * (1.0f - k) + k));

		float specular = (NdotL * _D * _F * _V);
	
		////-------------------------------------------------------------------------//
		float3 color = float3(1.f, 1.f, 1.f);
		float4 ambientcolor = float4(color * 0.5f, 1.0);
		float diffusefactor = dot(normal3, L);
		float4 diffusecolor = 0;
		
		if (diffusefactor > 0.0)
		{
			float diffuseintensity = 0.5f;
			diffusecolor = float4(color * diffuseintensity * diffusefactor, 1.0);
		}
		
		float4 light = diffusecolor * diffusefactor + ambientcolor;
		//float4 light = g_vLightDiffuse * (saturate(dot(normalize(g_vLightPos - vWorldPos) * -1.f, normal)) + (g_vLightAmbient * g_vMtrlAmbient));
		
		float3 CamToWorldDirection = normalize(vWorldPos.xyz - g_vCamPosition.xyz);
		float3 worldReflectDirection = reflect(CamToWorldDirection, normal3);
		
		float smoothness = 1 - Roughness;
		
		float4 cubeRef1 = g_SkyBoxTexutre.Sample(SkyBoxSampler, worldReflectDirection.xy);
		cubeRef1.a = 1.f;
		
		float InvMetalic = (1 - Metallic);
		InvMetalic = max(InvMetalic, 0.2f);
		float4 lightpower = InvMetalic * light * AO;
		
		if (g_shadow == true)
		{
			float4 shadow = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
			shadow = saturate(shadow + 0.1f);
			
			Out.vSpecular = (light * specular + cubeRef1) * Metallic * smoothness * shadow;
			Out.vShade = lightpower * shadow;
		}
		else
		{
			
			Out.vSpecular = (light * specular + cubeRef1) * Metallic * smoothness;
			Out.vShade = lightpower;
			//Out.vShade.a = 1.f;
		}
	}
	else
	{
		vector vReflect = reflect(normalize(g_vLightDir), normal);

		vector vLook = normalize(vWorldPos - g_vCamPosition);

		Out.vSpecular = ((g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f));
		
		float4 light = saturate(pow((dot(normalize(vector(g_vLightDir.xyz, 0.f)) * -1.f, normal) * 0.5f + 0.5f), lightpow) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient));
		Out.vShade = light;
		Out.vShade.a = 1.f;
	}
	
	return Out;
}

PS_OUT_LIGHTACC PS_MAIN_LIGHTACC_POINT(PS_IN In)
{
	PS_OUT_LIGHTACC Out = (PS_OUT_LIGHTACC) 0;

	vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector vShadowDesc = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	float fViewZ = vDepthDesc.y * 300.f;

	vector vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vLightDir = vWorldPos - g_vLightPos;
	float fDistance = length(vLightDir);

	float fAtt = saturate((g_fRange - fDistance) / g_fRange);

	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector vReflect = reflect(normalize(vLightDir), vNormal);

	vector vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular.xyz = ((g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt).xyz;
	
	return Out;
}

struct PS_OUT_VOLUMETRIC
{
	vector vColor : SV_TARGET0;
};

PS_OUT_VOLUMETRIC PS_MAIN_VOLUMETRIC(PS_IN In)
{
	PS_OUT_VOLUMETRIC Out = (PS_OUT_VOLUMETRIC) 0;
	
	float depth = max(In.vPosition.z, g_DepthTexture.SampleLevel(linear_clamp_sampler, In.vTexUV, 2));
	float3 P = GetPositionVS(In.vTexUV, depth,g_ProjMatrixInv);
	float3 V = float3(0.0f, 0.0f, 0.0f) - P;
	float cameradistance = length(V); 
	V /= cameradistance;
	
	float marcheddistance = 0;
	float3 acc = 0;
	
	const float3 L = g_vLightDir.xyz;
	
	float3 rayEnd = float3(0.0f, 0.0f, 0.0f);
	
	const uint sampleCount = 16;
	const float stepSize = length(P - rayEnd) / sampleCount;
	
	P = P + V * stepSize * dither(In.vPosition.xy);
	
	[loop]
	for (uint i = 0; i < sampleCount; ++i)
	{
		float4 posShadowMap = mul(float4(P, 1.0), g_shadowmatrix);
		float3 UVD = posShadowMap.xyz / posShadowMap.w;

		UVD.xy = 0.5 * UVD.xy + 0.5;
		UVD.y = 1.0 - UVD.y;
        
        [branch]
		if (IsSaturated(UVD.xy))
		{
			float attenuation = CalcShadowFactor_PCF3x3(shadowsampler, g_ShadowTexture, UVD, 2048, 1.0f);

			attenuation *= ExponentialFog(cameradistance - marcheddistance);

			acc += attenuation;

		}

		marcheddistance += stepSize;
		
		P = P + V * stepSize;
        
	}

	acc /= sampleCount;
	float3 color = float3(1.f, 1.f, 1.f);
	Out.vColor = max(0, float4(acc * color * 1.0f, 1));
	
	return Out;
}

PS_OUT_VOLUMETRIC PS_MAIN_SHADOW(PS_IN In)
{
	PS_OUT_VOLUMETRIC Out = (PS_OUT_VOLUMETRIC) 0;

	float2 uv = In.vTexUV;
	
	float2 depth = g_DepthTexture.Sample(DefaultSampler,In.vTexUV).xy;
	float fViewZ = depth.y * 300.f;

	vector vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = depth.x;
	vWorldPos.w = 1.0f;
	
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
	//float4 worldpos = GetWorldPosFromDepth(depth, In.vTexUV, 300.f, g_ProjMatrixInv, g_ViewMatrixInv);
	float4 lightpos = mul(vWorldPos, g_LightViewProj);
	
	float2 shadowUV = float2(0, 0);
	
	shadowUV.x = saturate((lightpos.x / lightpos.w) * 0.5f + 0.5f);
	shadowUV.y = saturate(-(lightpos.y / lightpos.w) * 0.5f + 0.5f);
	
	float4 lightDepth = g_ShadowMapTex.Sample(DefaultSampler, shadowUV);
	float lightDistance = lightDepth.x * 300.f;
	float objDistance = length(vWorldPos.xyz - g_vLightPos.xyz);
	
	float adjust = 0;
	
	float3 color = 1.f;
	if(objDistance - 0.1f < lightDistance)
	{
		color = 1;
	}
	else
	{
		color = 0;
	}
	
	Out.vColor = float4(color, 1.f);
	
	return Out;
}

struct PS_OUT_BLEND
{
	vector vColor : SV_TARGET0;
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND Out = (PS_OUT_BLEND) 0;
	
	float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	Out.vColor = color;
	
	if (Out.vColor.a == 0)
		discard;
	
	// 외곽선 효과 1.
	if (g_outline == true)
	{
		float fCoord[3] = { -1.f, 0.f, 1.f };
		float fLaplacianMask[9] =
		{
			-1.f, -1.f, -1.f,
			-1.f, 8.f, -1.f,
			-1.f, -1.f, -1.f
		};
		for (int i = 0; i < 9; ++i)
			Out.vColor += fLaplacianMask[i] * g_DiffuseTexture.Sample(DefaultSampler, (In.vTexUV + float2(fCoord[i / 3] / 1280.f, fCoord[i / 3] / 720.f)));
	}
	
	return Out;
}

PS_OUT_BLEND PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT_BLEND Out = (PS_OUT_BLEND) 0;
	
	float4 color = g_AlphaTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor = color;
	
	return Out;
}

//----------------------------------technique pass-----------------------------------//
technique11 DefaultTechnique
{
	// debug buffer rendering
	pass Viewport // 0
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	// lighting calculating  :  Directional Light
	pass Light_Directional // 1
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHTACC_DIRECTIONAL();
	}
// lighting calculating  :  Point Light
	pass Light_Point // 2
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHTACC_POINT();
	}

	// Render Targtes All blending
	pass Blend // 3
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	pass Alpha //4
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
	}

	pass Light_Directional_Volumetric // 5
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_Qude();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VOLUMETRIC();
	}

	pass Light_Directional_Shadow // 6
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

}
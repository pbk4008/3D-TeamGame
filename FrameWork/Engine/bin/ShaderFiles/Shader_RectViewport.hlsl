#include "Shader_Calculate.hpp"
#include "Shader_LightUtil.hlsli"
#include "Shader_DitherUtil.hlsli"
#include "Shader_ShadowUtil.hlsli"
#include "Shader_Post.hlsli"

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
	bool g_radial;
	bool g_distort;
	bool g_fog;
	bool g_motion;
};

cbuffer CountBuffer
{
	int g_RadialCnt;
	float g_delta;
	int g_MotionblurCnt;
};

cbuffer Fogbuffer
{
	float4 g_fogcolor = (float4) 0;
	float g_fogDenstiy = (float) 0;
	float g_fogDist = (float) 0;
	float g_fogstart = (float) 0;
	float g_fogend = (float) 0;
	float g_fogfalloff = (float) 0;
	bool g_fogType = (bool)0;
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
	matrix g_PointWorld;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
	
	matrix g_ProjMatrixInv;
	matrix g_ViewMatrixInv;
};

Texture2D g_SkyBoxTexture;

// Lighting

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_DepthTexture;
Texture2D g_MRATexture;

// belnding
Texture2D g_ShadowMapTex;
Texture2D g_ShadowTexture;

Texture2D g_AlphaTexture;
Texture2D g_BlurTexture;

Texture2D g_DistortionTex;
Texture2D g_VelocityTex;
Texture2D g_DissolveTex;
Texture2D g_RimLightTexture;

struct VS_IN
{
	half3 vPosition : POSITION;
	half2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	half4 vPosition : SV_POSITION;
	half2 vTexUV : TEXCOORD0;
};
struct VS_OUT_POINT
{
	half4 vPosition : SV_POSITION;
	half2 vTexUV : TEXCOORD0;
	half4 vClippos : TEXCOORD1;
};


VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;
	
	Out.vPosition = vector(In.vPosition, 1.f);

	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT_POINT VS_MAIN_POINT(VS_IN In)
{
	VS_OUT_POINT Out = (VS_OUT_POINT) 0;
	
	
	matrix vp = mul(g_PointWorld, g_ViewMatrix);
	matrix wvp = mul(vp, g_ProjMatrix);
	
	Out.vPosition = mul(half4(In.vPosition, 1.f), wvp);
	Out.vTexUV = In.vTexUV;
	Out.vClippos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	half4 vPosition : SV_POSITION;
	half2 vTexUV : TEXCOORD0;
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

struct PS_OUT_DIRLIGHTACC
{
	vector vShade : SV_TARGET0;
	vector vSpecular : SV_TARGET1;
};

struct PS_IN_POINTLIGHTACC
{
	half4 vPosition : SV_POSITION;
	half2 vTexUV : TEXCOORD0;
	half4 vClippos : TEXCOORD1;
};

struct PS_OUT_POINTLIGHTACC
{
	half4 vShade		: SV_TARGET0;
	half4 vSpecular	: SV_TARGET1;
	half4 vShadow		: SV_TARGET2;
};

PS_OUT_DIRLIGHTACC PS_MAIN_LIGHTACC_DIRECTIONAL(PS_IN In)
{
	PS_OUT_DIRLIGHTACC Out = (PS_OUT_DIRLIGHTACC) 0;
	
	half2 uvRT = In.vTexUV;
	
	half4 vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, uvRT);
	half4 vNormalDesc = g_NormalTexture.Sample(DefaultSampler, uvRT);
	half4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, uvRT);
	half3 MRA = g_MRATexture.Sample(DefaultSampler, uvRT).xyz;
	half fViewZ = vDepthDesc.y * 300.f;
	
	half3 normaltest = vNormalDesc.xyz;
	if (!any(normaltest))
		clip(0);

	half4 vWorldPos;
	vWorldPos.x = (uvRT.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (uvRT.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.x * fViewZ;
	vWorldPos.w = fViewZ;
	
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
	half4 normal = half4(normaltest * 2.f - 1.f, 0.f);
	
	if (g_bPBRHDR == true)
	{
		half3 normal3 = normalize(normal.xyz);
		half3 N = normal3;
		half3 V = normalize(g_vCamPosition.xyz - vWorldPos.xyz);
		half3 L = g_vLightDir.xyz * -1;
		half F0 = 0.93;
		
		half specular = LightingGGX_Ref(N, V, L, F0, MRA.g);
	
		////-------------------------------------------------------------------------//
		half3 color = half3(0.97f, 0.95f, 0.8f);
		half ambientintensity = 0.2f;
		half4 light = CalcLightInternal(color, ambientintensity, g_vCamPosition.xyz, g_vLightDir.xyz, vWorldPos.xyz, normal3);
		//half4 light = g_vLightDiffuse * (saturate(dot(normalize(g_vLightPos - vWorldPos) * -1.f, normal)) + (g_vLightAmbient * g_vMtrlAmbient));
		//half lightpow = 2.f;
		//half4 light = saturate(pow((dot(normalize(vector(g_vLightDir.xyz, 0.f)) * -1.f, normal) * 0.5f + 0.5f), lightpow) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient));
		
		half3 CamToWorldDirection = normalize(vWorldPos.xyz - g_vCamPosition.xyz);
		half3 worldReflectDirection = reflect(CamToWorldDirection, normal3);
		
		half4 cubeRef1 = g_SkyBoxTexture.Sample(SkyBoxSampler, worldReflectDirection.xy);
		
		half smoothness = 1 - MRA.g;
		half InvMetalic = (1 - MRA.r);
		InvMetalic = max(InvMetalic, 0.5f);
		half4 lightpower = InvMetalic * light * MRA.b;
		
		if (g_shadow == true)
		{
			half4 shadow = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
			shadow = saturate(shadow + 0.5f);
			
			Out.vSpecular = saturate((light * specular + cubeRef1) * MRA.r * smoothness * shadow);
			Out.vShade = lightpower * shadow;
		}
		else
		{
			Out.vSpecular = saturate((light * specular + cubeRef1) * MRA.r * smoothness);
			Out.vShade = saturate(lightpower);
		}
	}
	else
	{
		half lightpow = 1.f;
		vector vReflect = reflect(normalize(g_vLightDir), normal);

		vector vLook = normalize(vWorldPos - g_vCamPosition);

		Out.vSpecular = ((g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f));
		
		half4 light = saturate(pow((dot(normalize(vector(g_vLightDir.xyz, 0.f)) * -1.f, normal) * 0.5f + 0.5f), lightpow) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient));
		Out.vShade = light;
		Out.vShade.a = 1.f;
	}
	
	return Out;
}

PS_OUT_POINTLIGHTACC PS_MAIN_LIGHTACC_POINT(PS_IN In)
{
	PS_OUT_POINTLIGHTACC Out = (PS_OUT_POINTLIGHTACC) 0;
	
	vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector vShadowDesc = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	half fViewZ = vDepthDesc.y * 300.f;

	vector vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vLightDir = vWorldPos - g_vLightPos;
	half fDistance = length(vLightDir);

	half fAtt = saturate((g_fRange - fDistance) / g_fRange);

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

PS_OUT_VOLUMETRIC PS_MAIN_SHADOW(PS_IN In)
{
	PS_OUT_VOLUMETRIC Out = (PS_OUT_VOLUMETRIC) 0;

	half2 uv = In.vTexUV;
	
	half2 depth = g_DepthTexture.Sample(DefaultSampler,In.vTexUV).xy;
	half fViewZ = depth.y * 300.f;

	vector vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = depth.x;
	vWorldPos.w = 1.0f;
	
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
	half4 lightpos = mul(vWorldPos, g_LightViewProj);
	
	half2 shadowUV = half2(0, 0);
	
	shadowUV.x = saturate((lightpos.x / lightpos.w) * 0.5f + 0.5f);
	shadowUV.y = saturate(-(lightpos.y / lightpos.w) * 0.5f + 0.5f);
	
	half4 lightDepth = g_ShadowMapTex.Sample(DefaultSampler, shadowUV);
	half lightDistance = lightDepth.x * 300.f;
	half objDistance = length(vWorldPos.xyz - g_vLightPos.xyz);
	
	half adjust = 0;
	
	half3 color = 1.f;
	if(objDistance - 0.1f < lightDistance)
	{
		color = 1;
	}
	else
	{
		color = 0;
	}
	
	Out.vColor = half4(color, 1.f);
	
	return Out;
}

struct PS_OUT_BLEND
{
	vector vColor : SV_TARGET0;
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND Out = (PS_OUT_BLEND) 0;
	
	half4 depth = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	half4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	half fViewZ = depth.y * 300.f;
	
	Out.vColor = color;
	
	if (g_motion == true)
	{
		half4 velocity = g_VelocityTex.Sample(DefaultSampler, In.vTexUV);
		velocity.xy /= (half) g_MotionblurCnt;
		int cnt = 1;
		half4 bcolor;
		for (int i = cnt; i < g_MotionblurCnt; ++i)
		{
			bcolor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV + velocity.xy * (half) i);
			if (velocity.z < depth.x + 0.04f)
			{
				cnt++;
				Out.vColor += bcolor;
			}
		}
		Out.vColor /= (half) cnt;
	}
	
	if (g_distort == true)
	{
		Out.vColor = Distortion(g_DistortionTex, g_DiffuseTexture, DefaultSampler, In.vTexUV);
	}
	
	if (g_radial == true)
	{	
		Out.vColor.rgb = Radialblur(g_DiffuseTexture,DefaultSampler,In.vTexUV,g_RadialCnt);
	}
	
	if(g_outline == true)
	{
		half4 rim = g_RimLightTexture.Sample(DefaultSampler, In.vTexUV);
		Out.vColor = Outline(g_RimLightTexture, DefaultSampler, In.vTexUV, Out.vColor);
	}
	
	if (Out.vColor.a == 0)
		discard;
	
	if (g_fog == true)
	{
		half4 worldpos;
		worldpos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
		worldpos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
		worldpos.z = depth.x * fViewZ;
		worldpos.w = fViewZ;
		worldpos = mul(worldpos, g_ProjMatrixInv);
		worldpos = mul(worldpos, g_ViewMatrixInv);
		float fog = 0.0f;
		if (g_fogType == EXPONENTIAL_FOG)
		{
			fog = ExponentialFog(worldpos,g_vCamPosition,g_fogstart,g_fogDenstiy);
		}
		else if (g_fogType == EXPONENTIAL_HEIGHT_FOG)
		{
			fog = ExponentialHeightFog(worldpos,g_vCamPosition,g_fogstart,g_fogDenstiy,g_fogfalloff);
		}
		Out.vColor = lerp(Out.vColor, g_fogcolor, fog);
		//half fogfactor = 1.0 / pow(2.71828, saturate((fViewZ - g_fogDist) * g_fogDenstiy));
		//Out.vColor = fogfactor * Out.vColor + (1.0 - fogfactor) * g_fogcolor;
	}
	
	return Out;
}

PS_OUT_BLEND PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT_BLEND Out = (PS_OUT_BLEND) 0;
	
	half4 color = g_AlphaTexture.Sample(DefaultSampler, In.vTexUV);
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

	pass Light_Directional_Shadow // 5
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

}
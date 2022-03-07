#include "Shader_Calculate.hpp"

#pragma pack_matrix(row_major);


sampler DefaultSampler = sampler_state
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer ShaderCheck
{
	bool g_bShadow;
	bool g_bPBRHDR;
};

cbuffer LightDesc
{
	vector		g_vLightDir;
	vector		g_vLightPos;
	float		g_fRange;
	vector		g_vLightDiffuse;
	vector		g_vLightAmbient;
	vector		g_vLightSpecular;
};

cbuffer MaterialDesc
{
	vector		g_vMtrlAmbient = (vector)1.f;
	vector		g_vMtrlDiffuse = (vector)1.f;
	vector		g_vMtrlSpecular = (vector)1.f;
};

cbuffer CameraDesc
{
	vector		g_vCamPosition;
};

cbuffer MatrixInverse
{
	matrix		g_MainCamProjMatrix;
	matrix		g_ProjMatrixInv;
	matrix		g_ViewMatrixInv;
};

// ?? 
texture2D	g_DepthTexture;

// Lighting
texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;
texture2D	g_PositionTexture;

texture2D	g_Metallic;
texture2D	g_Roughness;
texture2D	g_AO;

// belnding
texture2D	g_ShadeTexture;
texture2D	g_SpecularTexture;
texture2D	g_ShadowTexture;

/* 1. m_pDeviceContext->DrawIndexed() */
/* 2. 인덱스가 가지고 있던 인덱스 세개에 해당하는 정점 세개를 정점버퍼로부터 얻어온다. */
/* 3. VS_MAIN함수를 호출하면서 가져온 정점 세개중 하나씩 전달해준다.  */

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	Out.vPosition = vector(In.vPosition, 1.f);

	Out.vTexUV = In.vTexUV;

	return Out;
}
/* SV_POSITION을 가진 데잍처에대해서만 원근투영.(정점의 w값으로 xyzw를 나눈다.) */
/* 뷰포트로 변환한다. */
/* 래스터라이즈.(둘러쌓여진 정점의 정보를 바탕으로 하여. 픽셀정보를 생성한다.) */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;	
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};


/* 1. 픽셀의 색을 결정한다. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	return Out;	
}

struct PS_OUT_LIGHTACC
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHTACC PS_MAIN_LIGHTACC_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHTACC Out = (PS_OUT_LIGHTACC) 0;
	
	vector vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	
	float Metallic = g_Metallic.Sample(DefaultSampler, In.vTexUV).r;
	float Roughness = g_Roughness.Sample(DefaultSampler, In.vTexUV).r;
	float AO = g_AO.Sample(DefaultSampler, In.vTexUV).r;
	float fViewZ = vDepthDesc.y * 300.f;
	
	vector vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.f;
	
	//vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
	vWorldPos.xyz = (vWorldPos / vWorldPos.w).xyz;
	vWorldPos.w = 1.f;
	
	/* 0 ~ 1*/ /* -1 ~ 1*/
	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	float lightpower = 1.f;
	//Out.vShade = saturate(dot(normalize(vector(g_vLightDir.xyz, 0.f)) * -1.f, vNormal ) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade = saturate(pow((dot(normalize(vector(g_vLightDir.xyz, 0.f)) * -1.f, vNormal) * 0.5f + 0.5f), lightpower) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient));
	//Out.vShade.a = 1.f;
	
	if (g_bPBRHDR == true)
	{
		float gamma = 2.2; // liner space 에서 빛계산을 하는것이 중요 그래서 gamma correction 된 공간에서 계산을 하는것이다.
		
		float3 diffuse = vDiffuseDesc.rgb;
		diffuse = pow((diffuse), gamma);
		
		float3 N = normalize(vNormal).rgb;
		float3 V = normalize(g_vCamPosition - vWorldPos).rgb;
		
		float3 F0 = 0.04f; // F0 : 반사율 - 0.04는 비금속 표면에 대해 시각적으로 정확해 보입니다.
		F0 = lerp(F0, diffuse, Metallic);
		
		float3 Lo = float3(0.f, 0.f, 0.f);
		
		float3 L = normalize(g_vLightPos - vWorldPos).rgb;
		float3 H = normalize(V + L);
		float distance = length(g_vLightPos - vWorldPos);
		float attenuation = 1.f / (distance * distance);
		//float3 reflectratio = float3(1.f, 1.f, 1.f) * attenuation // 반사율 거리 비례 
		float3 reflectratio = float3(1.f, 1.f, 1.f); // 반사율 full power
		
		float NDF = NormalDistributionGGXTR(N, H, Roughness); // 면의 거칠기에 따른 빛 에너지 '근사량!!'
		float G = GeometrySmit(N, V, L, Roughness); // 각각의 면에 따른 상호적인 '감쇄량!!'
		float3 F = FresnelSchlick(saturate(dot(H, V)), F0); // 프레넬 방정식을 통한 '반사량!!' 시야각에 따른 '반사율'을 구하기 위해 필요
		
		float3 kS = F; // 반사량
		float3 kD = 1.f - kS; // 반사량을 통한 굴절 에너지 유도
		kD *= 1.0 - 1.0; // 추가적인 금속 재질정도를 곱연산
		// 왜 1.f 에서 Metallic 의 값을 빼주는건가 ?? 재질이 완전히 metallic 이면 굴절을 하지 않고 반사가 되기 때문에 0으로 바꾸는 것
		// 이렇게 해서 나온 겨로가값이 바로 최종 '정반사량!!!'
		
		// BRDF 
		float3 numerator = NDF * G * F; // 분자 
		float denominator = 4.0 * saturate(dot(N, V)) * saturate(dot(N, L)) + 0.0001; // 분모
		float3 specular = numerator / denominator;
		
		float NdotL = saturate(dot(N, L));
		Lo = ((kD * diffuse / PI) + specular) * reflectratio * NdotL;
		
		float3 ambient = (g_vLightAmbient * g_vMtrlAmbient).rgb * diffuse * AO;
		float3 PBR_Specular = ambient + (Lo * (g_vLightDiffuse * g_vMtrlDiffuse).rgb);
		//PBR_Specular = PBR_Specular / (PBR_Specular + float(1.f).xxx);
		PBR_Specular = pow((PBR_Specular), 1.0 / 2.2);
		
		//Out.vSpecular.rgb = PBR_Specular.rgb/*float4(PBR_Specular.rgb, 1.f)*/;
		Out.vSpecular.rgb = float4(PBR_Specular.rgb, 1.f).rgb;
		
	}
	else
	{
		vector vReflect = reflect(normalize(g_vLightDir), vNormal);

		vector vLook = normalize(vWorldPos - g_vCamPosition);

		Out.vSpecular.xyz = ((g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f)).xyz;
	}
	
	return Out;
}

PS_OUT_LIGHTACC PS_MAIN_LIGHTACC_POINT(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadowDesc = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.y * 300.f;

	vector		vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vLightDir), vNormal);

	vector		vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular.xyz = ((g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt).xyz;
	
	return Out;
}

struct PS_OUT_BLEND
{
	vector		vColor : SV_TARGET0;
};


PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND		Out = (PS_OUT_BLEND)0;

	vector		vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vSpecularDesc = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadowTexture = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	
	if (g_bShadow == true)
		Out.vColor = vDiffuseDesc * vShadeDesc * vShadowTexture + vSpecularDesc;
	else if (g_bShadow == false)
		Out.vColor = vDiffuseDesc * vShadeDesc + vSpecularDesc;

	// 외곽선 효과
	//float fCoord[3] = { -1.f, 0.f, 1.f };

	//float fLaplacianMask[9] =
	//{
	//	-1.f, -1.f, -1.f,
	//	-1.f, 8.f, -1.f,
	//	-1.f, -1.f, -1.f
	//};
	
	//for (int i = 0; i < 9; ++i)
	//	Out.vColor += fLaplacianMask[i] * g_DiffuseTexture.Sample(DefaultSampler, (In.vTexUV + float2(fCoord[i / 3] / 1280.f, fCoord[i / 3] / 720.f)));
	

	if (Out.vColor.a == 0.f)
		discard;

	return Out;
}

//----------------------------------technique pass-----------------------------------//
technique11			DefaultTechnique
{
	// debug buffer rendering
	pass Viewport
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}

	// lighting calculating  :  Directional Light
	pass Light_Directional
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_LIGHTACC_DIRECTIONAL();
	}
// lighting calculating  :  Point Light
	pass Light_Point
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(OneBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_LIGHTACC_POINT();
	}

	// Render Targtes All blending
	pass Blend
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZTestDiable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_BLEND();
	}
}





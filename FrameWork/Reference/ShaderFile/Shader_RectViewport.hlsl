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
/* 2. �ε����� ������ �ִ� �ε��� ������ �ش��ϴ� ���� ������ �������۷κ��� ���´�. */
/* 3. VS_MAIN�Լ��� ȣ���ϸ鼭 ������ ���� ������ �ϳ��� �������ش�.  */

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
/* SV_POSITION�� ���� ����ó�����ؼ��� ��������.(������ w������ xyzw�� ������.) */
/* ����Ʈ�� ��ȯ�Ѵ�. */
/* �����Ͷ�����.(�ѷ��׿��� ������ ������ �������� �Ͽ�. �ȼ������� �����Ѵ�.) */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;	
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};


/* 1. �ȼ��� ���� �����Ѵ�. */
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
		float gamma = 2.2; // liner space ���� ������� �ϴ°��� �߿� �׷��� gamma correction �� �������� ����� �ϴ°��̴�.
		
		float3 diffuse = vDiffuseDesc.rgb;
		diffuse = pow((diffuse), gamma);
		
		float3 N = normalize(vNormal).rgb;
		float3 V = normalize(g_vCamPosition - vWorldPos).rgb;
		
		float3 F0 = 0.04f; // F0 : �ݻ��� - 0.04�� ��ݼ� ǥ�鿡 ���� �ð������� ��Ȯ�� ���Դϴ�.
		F0 = lerp(F0, diffuse, Metallic);
		
		float3 Lo = float3(0.f, 0.f, 0.f);
		
		float3 L = normalize(g_vLightPos - vWorldPos).rgb;
		float3 H = normalize(V + L);
		float distance = length(g_vLightPos - vWorldPos);
		float attenuation = 1.f / (distance * distance);
		//float3 reflectratio = float3(1.f, 1.f, 1.f) * attenuation // �ݻ��� �Ÿ� ��� 
		float3 reflectratio = float3(1.f, 1.f, 1.f); // �ݻ��� full power
		
		float NDF = NormalDistributionGGXTR(N, H, Roughness); // ���� ��ĥ�⿡ ���� �� ������ '�ٻ緮!!'
		float G = GeometrySmit(N, V, L, Roughness); // ������ �鿡 ���� ��ȣ���� '���ⷮ!!'
		float3 F = FresnelSchlick(saturate(dot(H, V)), F0); // ������ �������� ���� '�ݻ緮!!' �þ߰��� ���� '�ݻ���'�� ���ϱ� ���� �ʿ�
		
		float3 kS = F; // �ݻ緮
		float3 kD = 1.f - kS; // �ݻ緮�� ���� ���� ������ ����
		kD *= 1.0 - 1.0; // �߰����� �ݼ� ���������� ������
		// �� 1.f ���� Metallic �� ���� ���ִ°ǰ� ?? ������ ������ metallic �̸� ������ ���� �ʰ� �ݻ簡 �Ǳ� ������ 0���� �ٲٴ� ��
		// �̷��� �ؼ� ���� �ܷΰ����� �ٷ� ���� '���ݻ緮!!!'
		
		// BRDF 
		float3 numerator = NDF * G * F; // ���� 
		float denominator = 4.0 * saturate(dot(N, V)) * saturate(dot(N, L)) + 0.0001; // �и�
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

	// �ܰ��� ȿ��
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





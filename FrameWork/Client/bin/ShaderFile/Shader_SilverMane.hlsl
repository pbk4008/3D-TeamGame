#include "Shader_RenderState.hpp"

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

cbuffer Matrices
{
	matrix		g_WorldMatrix  = (matrix)0;
	matrix		g_ViewMatrix;	
	matrix		g_ProjMatrix;
};

struct BoneMatrixArray
{
	matrix		Bone[256];
};

cbuffer	BoneMatricesBuffer
{
	BoneMatrixArray		g_BoneMatrices;
};

cbuffer LightBuffer
{
	matrix g_LightView;
	matrix g_LightProj;
	float3 g_LightPos;
};

texture2D	g_ShadowTexture;

texture2D	g_DiffuseTexture;
texture2D	g_BiNormalTexture;

texture2D	g_MRATexture;
texture2D	g_OMERTexture;
texture2D	g_CEOTexture;

texture2D	g_NewHairTexture;


float3 Normalmapping(float3 normaltex, float3x3 tbn)
{	
	normaltex = normaltex * 2 - 1;
	normaltex = normalize(normaltex);
	
	normaltex = normalize(mul(normaltex, tbn));
	normaltex = normaltex * 0.5f + 0.5f;
	
	return normaltex;
}


struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	float3	vBiNormal : BINORMAL;
	uint4	vBlendIndex : BLENDINDEX;
	float4	vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float4	vBiNormal	: BINORMAL;
	float4	vUvDepth	: TEXCOORD0;
	//float2	vTexUV : TEXCOORD0;
	//float4	vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN_ANIM(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	


	matrix			matWV, matWVP;	

	float		fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix			BoneMatrix =	g_BoneMatrices.Bone[In.vBlendIndex.x] * In.vBlendWeight.x +
									g_BoneMatrices.Bone[In.vBlendIndex.y] * In.vBlendWeight.y +
									g_BoneMatrices.Bone[In.vBlendIndex.z] * In.vBlendWeight.z +
									g_BoneMatrices.Bone[In.vBlendIndex.w] * In.vBlendWeight.w;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
		
	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vPosition = mul(vPosition, matWVP);
	Out.vPosition = vPosition;

	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector		vBiNoraml = mul(vector(In.vBiNormal, 0.f), BoneMatrix);
	vector		vTantent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vBiNoraml, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTantent, g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	
	return Out;

}
// VS_SHADOWMAP
//*---------------------------------------------------------------------------------------------*

struct VS_OUT_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV,matWVP;
	
	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix BoneMatrix = g_BoneMatrices.Bone[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.Bone[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.Bone[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.Bone[In.vBlendIndex.w] * In.vBlendWeight.w;
	
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);
	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vClipPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	
	//matrix matVP = mul(g_LightView, g_LightProj);
	//float4 worldpos = mul(vPosition, g_WorldMatrix);
	//Out.vClipPos = worldpos;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

// VS_SHADE_SHADOW
//*---------------------------------------------------------------------------------------------*
struct VS_OUT_SHADESHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vLightPosition : TEXCOORD1;
};

VS_OUT_SHADESHADOW VS_MAIN_SHADESHADOW(VS_IN In)
{
	VS_OUT_SHADESHADOW Out = (VS_OUT_SHADESHADOW) 0.f;
	
	matrix matWV, matWVP, matLightWV, matLightWVP;
	
	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix BoneMatrix = g_BoneMatrices.Bone[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.Bone[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.Bone[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.Bone[In.vBlendIndex.w] * In.vBlendWeight.w;
	
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	matLightWV = mul(g_WorldMatrix, g_LightView);
	matLightWVP = mul(matLightWV, g_LightProj);
	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLightPosition = mul(vPosition, matLightWVP);
	//Out.vLightPosition = Out.vPosition;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

// SHADOWMAP
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	float fDepth = In.vClipPos.z / In.vClipPos.w;
	
	float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	float Alpha = 1.f;
	
	if (color.a < 0.1f)
	{
		Alpha = color.a;
	}
	
	Out.vShadowDepthMap = vector(fDepth.xxx, Alpha);
	
	//float4 shadow = 1;
	//float zFar = 1 / 300.f;
	//shadow.xyz = length(In.vClipPos.xyz - g_LightPos) * zFar;
	//Out.vShadowDepthMap = shadow;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

// SHADE SHADOW
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADESHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vLightPosition : TEXCOORD1;
};

struct PS_OUT_SHADESHADOW
{
	vector vShadeShadow : SV_TARGET0;
};

PS_OUT_SHADESHADOW PS_MAIN_SHADESHADOW(PS_IN_SHADESHADOW In)
{
	PS_OUT_SHADESHADOW Out = (PS_OUT_SHADESHADOW) 0.f;
	
	float fOut = 1.f;
	
	vector Diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float Bias = 0.001f;
	float ShadowIntensity = 0.6f;

	float CurrentDepth = In.vLightPosition.z / In.vLightPosition.w;
	
	float2 ShadowUV = In.vLightPosition.xy / In.vLightPosition.w;
	
	if (ShadowUV.x < -fOut || ShadowUV.x > fOut || ShadowUV.y < -fOut || ShadowUV.y > fOut || CurrentDepth < -fOut || CurrentDepth > fOut)
	{
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
	}
	else
	{
		ShadowUV.y = (ShadowUV.y * -1.f);
		ShadowUV = ShadowUV * 0.5f + 0.5f;

		float shadowDepth;
		shadowDepth = g_ShadowTexture.Sample(ClampSampler, ShadowUV).r;
		
		Out.vShadeShadow = float4(1.f, 1.f, 1.f, 1.f);
		if (CurrentDepth > shadowDepth + Bias)
		{
			Out.vShadeShadow.rgb *= ShadowIntensity;

			if (Diffuse.a < 0.9f)
			{
				Out.vShadeShadow.a = Diffuse.a;
			}
		}
		else
		{
			if (Diffuse.a < 0.9f)
			{
				Out.vShadeShadow.a = Diffuse.a;
			}
		}
	}
	return Out;
	
}
//*---------------------------------------------------------------------------------------------*

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT
{
	float4 diffuse	: SV_TARGET0;
	float4 normal	: SV_TARGET1;
	float4 depth	: SV_TARGET2;
	float4 M		: SV_Target3;
	float4 R		: SV_Target4;
	float4 A		: SV_Target5;
	float4 E		: SV_Target6;
};

PS_OUT PS_MAIN_TOP(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);

	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	//float4 Ecolor = float4(0.98, 0.23, 0.19, 0.f);
	float4 Ecolor = float4(0.498f, 0.9411f, 0.8196f,0.f);
	float Epower = 1.0f;
	
	float accvalue = diffuse.r + diffuse.g - diffuse.b;
	if(accvalue > 0.6f)
	{
		Out.diffuse = float4(0.9f, 0.57f, 0.f, 1.f) * accvalue;
		Out.normal = float4(normal, 0.f);
		float metalic = omer.g * 0.05f + mra.r;
		Out.M = float4(metalic.xxx, 1.f);
		float roughness = omer.a + mra.g * 3.3f - mra.r - mra.b - (1 - omer.g);
		Out.R = float4(roughness.xxx, 1.f);
		//float ao = 1.f;
		//float ao = (omer.r - 0.3f) * 1.f;;
		float ao = mra.b/* - omer.r*/;
		Out.A = float4(ao.xxx, 1.f);
		//Out.E = float4(0.11f, 0.05f, 0.f, 1.f);
		Out.E = Ecolor * Epower * omer.b;
	}
	else
	{
		//float4 Ecolor = float4(0.98, 0.23, 0.19,1.f);
		//float	Epower = 0.7f;
		
		Out.normal = float4(normal,0.f);
		Out.diffuse = diffuse;
		
		float metalic = omer.g * 0.05f + mra.r;
		Out.M = float4(metalic.xxx, 1.f);
		float roughness = omer.a + mra.g * 3.f - mra.r - mra.b - (1 - omer.g);
		Out.R = float4(roughness.xxx, 1.f);
		//float ao = (omer.r - 0.3f) * 1.f;
		float ao = mra.b/* - omer.r*/;
		Out.A = float4(ao.xxx, 1.f);
		Out.E = Ecolor * Epower * omer.b;

	}
	
	return Out;	
}

PS_OUT PS_MAIN_DOWN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	
	Out.normal = float4(normal,0.f);
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	float metalic = mra.r;
	Out.M = float4(metalic.xxx, 1.f);
	
	float roughness = 0.5f + mra.g * 3.f - mra.r * 2;
	Out.R = float4(roughness.xxx, 1.f);
	
	float ao = ceo.b * 1.f * mra.b;
	Out.A = float4(ao.xxx, 1.f);
	
	float E = ceo.g * 0.5f;
	
	Out.E = float4(E.xxx, 0.f);
	
	return Out;
}

PS_OUT PS_MAIN_CLOAK(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	Out.normal = float4(normal, 0.f);
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	
	Out.M = float4(omer.g, omer.g, omer.g, 1);
	Out.R = float4(omer.a, omer.a, omer.a, 1);
	Out.A = float4(omer.r, omer.r, omer.r, 1);
	Out.E = float4(omer.b, omer.b, omer.b, 1);
	
	return Out;
}

PS_OUT PS_MAIN_HAIR(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	//g_NewHairTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 diffuse = g_NewHairTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float4 diffuse2 = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	diffuse.rgb = diffuse.rgb/* + float3(0.66, 0.66, 0.66)*/;
	float4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse.xyz = diffuse * 0.5f + 0.5f;
	Out.diffuse.w = diffuse2.w;
	
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.normal = float4(normal, 0.f);
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	Out.M = float4(omer.g, omer.g, omer.g, 1);
	Out.R = float4(omer.a, omer.a, omer.a, 1);
	Out.A = float4(omer.r, omer.r, omer.r, 1);
	Out.E = float4(omer.b, omer.b, omer.b, 1);
	
	if(Out.diffuse.a == 0)
		discard;
	
	return Out;
}

technique11			DefaultTechnique
{	
	pass SliverManeTop //------------------------------------------------------------------------------------0 Top
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_TOP();
	}

	pass SliverManeDown //------------------------------------------------------------------------------------1 Down
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOWN();
	}

	pass SliverManeCloak //------------------------------------------------------------------------------------2 Cloak
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOAK();
	}

	pass SliverManeHair //------------------------------------------------------------------------------------3 Hair
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HAIR();
	}


	pass ShadowANIM //-----------------------------------------------------------------------------------------4 Anim ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass ShadeShadow //-----------------------------------------------------------------------------------------5 Anim Shade_Shadow
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADESHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADESHADOW();
	}
}





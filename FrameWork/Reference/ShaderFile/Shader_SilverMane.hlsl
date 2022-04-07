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

cbuffer Colorbuffer
{
	float4 g_color;
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
	float3 worldpos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV,matWVP;
	
	half fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


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
	
	half4 worldpos = mul(vPosition, g_WorldMatrix);
	Out.worldpos = worldpos.xyz;
	
	return Out;
}

// SHADOWMAP
//*---------------------------------------------------------------------------------------------*
struct PS_IN_SHADOW
{
	half4 vPosition : SV_Position;
	half2 vTexUV : TEXCOORD0;
	half4 vClipPos : TEXCOORD1;
	half3 worldpos : TEXCOORD2;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	float4 color = 1;
	float OneDividzFar = 1 / 300.f;
	color.xyz = length(In.worldpos - g_LightPos) * OneDividzFar;
	Out.vShadowDepthMap = color;
	
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
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);

	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	//half4 Ecolor = float4(0.498f, 0.9411f, 0.8196f, 0.f);
	half Epower = 1.0f;
	
	half accvalue = diffuse.r + diffuse.g - diffuse.b;
	if(accvalue > 0.6f)
	{
		Out.diffuse = half4(0.9f, 0.57f, 0.f, 1.f) * accvalue;
		Out.normal = half4(normal, 0.f);
		half metalic = omer.g * 0.05f + mra.r;
		Out.M = half4(metalic.xxx, 1.f);
		half roughness = omer.a + mra.g * 3.3f - mra.r - mra.b - (1 - omer.g);
		Out.R = half4(roughness.xxx, 1.f);
		half ao = mra.b;
		Out.A = half4(ao.xxx, 1.f);
		Out.E = g_color * Epower * omer.b;
	}
	else
	{	
		Out.normal = half4(normal, 0.f);
		Out.diffuse = diffuse;
		
		half metalic = omer.g * 0.05f + mra.r;
		Out.M = half4(metalic.xxx, 1.f);
		half roughness = omer.a + mra.g * 3.f - mra.r - mra.b - (1 - omer.g);
		Out.R = half4(roughness.xxx, 1.f);
		half ao = mra.b;
		Out.A = half4(ao.xxx, 1.f);
		Out.E = g_color * Epower * omer.b;

	}
	
	return Out;	
}

PS_OUT PS_MAIN_DOWN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	
	Out.normal = half4(normal, 0.f);
	
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	half metalic = mra.r;
	Out.M = half4(metalic.xxx, 1.f);
	
	half roughness = 0.5f + mra.g * 3.f - mra.r * 2;
	Out.R = half4(roughness.xxx, 1.f);
	
	half ao = ceo.b * 1.f * mra.b;
	Out.A = half4(ao.xxx, 1.f);
	
	half E = ceo.g * 0.5f;
	
	Out.E = half4(E.xxx, 0.f);
	
	return Out;
}

PS_OUT PS_MAIN_CLOAK(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	Out.normal = half4(normal, 0.f);
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	
	Out.M = half4(omer.g, omer.g, omer.g, 1);
	Out.R = half4(omer.a, omer.a, omer.a, 1);
	Out.A = half4(omer.r, omer.r, omer.r, 1);
	Out.E = half4(omer.b, omer.b, omer.b, 1);
	
	return Out;
}

PS_OUT PS_MAIN_HAIR(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_NewHairTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 diffuse2 = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	diffuse.rgb = diffuse.rgb;
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse.xyz = diffuse * 0.5f + 0.5f;
	Out.diffuse.w = diffuse2.w;
	
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.normal = half4(normal, 0.f);
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	Out.M = half4(omer.g, omer.g, omer.g, 1);
	Out.R = half4(omer.a, omer.a, omer.a, 1);
	Out.A = half4(omer.r, omer.r, omer.r, 1);
	Out.E = half4(omer.b, omer.b, omer.b, 1);
	
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

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_TOP();
	}

	pass SliverManeDown //------------------------------------------------------------------------------------1 Down
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOWN();
	}

	pass SliverManeCloak //------------------------------------------------------------------------------------2 Cloak
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOAK();
	}

	pass SliverManeHair //------------------------------------------------------------------------------------3 Hair
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HAIR();
	}


	pass ShadowANIM //-----------------------------------------------------------------------------------------4 Anim ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
}





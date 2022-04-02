#include "Shader_RenderState.hpp"


cbuffer Matrices
{
	matrix g_WorldMatrix = (matrix) 0;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
	vector g_campos;
};

textureCUBE g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler SkyBoxSampler = sampler_state
{
	filter = comparison_min_mag_mip_point;
	AddressU = mirror;
	AddressV = mirror;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float3 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;

	matrix matWV, matWVP, matVP, matworld;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matVP = mul(g_ViewMatrix, g_ProjMatrix);
	
	matworld = (matrix) 0;
	
	//matworld._11_22_33_44 = float4(1, 1, 1, 1);
	//matworld._41_42_43 = float3(g_campos.x, g_campos.y, g_campos.z);
	
	//matWVP = mul(matworld, matVP);
	matWVP = mul(matWV, g_ProjMatrix);
	Out.vPosition = mul(float4(In.vPosition, 1), matWVP)/*.xyww*/;
	Out.vTexUV = In.vTexUV;

	return Out;

}


struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float3 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;

	Out.vColor = g_DiffuseTexture.Sample(SkyBoxSampler, In.vTexUV);

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

	Out.diffuse = g_DiffuseTexture.Sample(SkyBoxSampler, In.vTexUV);

	Out.depth = float4(In.vPosition.z / In.vPosition.w, In.vPosition.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1,0);
	
	Out.M = float4(0,0,0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
	Out.E = float4(0, 0, 0, 1);
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Normal
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZWriteDisable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalTest
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZWriteDisable, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST();
	}
}

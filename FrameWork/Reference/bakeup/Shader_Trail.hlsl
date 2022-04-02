#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix  = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

texture2D	g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
};

 VS_OUT_TRAIL VS_MAIN_TRAIL(VS_IN In)
{
	VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	
	return Out;
}

struct PS_IN_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT_TRAIL
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	float4 M : SV_Target3;
	float4 R : SV_Target4;
	float4 A : SV_Target5;
	float4 E : SV_Target6;
};

PS_OUT_TRAIL PS_MAIN_TRAIL(PS_IN_TRAIL In)
{
	PS_OUT_TRAIL Out = (PS_OUT_TRAIL) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse = diffuse;

	if (Out.diffuse.a == 0)
		discard;
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1, 0);
	
	Out.M = float4(0, 0, 0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
	float4 color = float4(0.99f, 0.43f, 0.2f, 0.3f);
	float4 power = 0.8f;
	Out.E = color * power;

	return Out;
}



technique11 DefaultTechnique
{
	pass TrailRect
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}
}

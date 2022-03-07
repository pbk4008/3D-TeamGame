#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

cbuffer BrushDesc
{
	vector		g_vBrushPos;
	float		g_fRadius;
	bool		g_bSameHeight;
};

texture2D	g_DiffuseSourTexture;
texture2D	g_DiffuseDestTexture;
texture2D   g_FilterTexture;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorld : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);


	Out.vWorld = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorld : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	 vSourDiffuse = g_DiffuseSourTexture.Sample(DefaultSampler, In.vTexUV * 10.f);
	vector vDestDiffuse = g_DiffuseDestTexture.Sample(DefaultSampler, In.vTexUV * 20.f);
	vector vFilterColor = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);
	vector	 vBrushColor = (vector)0;

	vector	 vDiffuse = vSourDiffuse* vFilterColor.r + vDestDiffuse*(1.f- vFilterColor.r);
	//원의 방정식
	if (!g_bSameHeight)
	{
		if (pow((In.vWorld.x - g_vBrushPos.x), 2) + pow((In.vWorld.z - g_vBrushPos.z), 2) < pow(g_fRadius, 2))
		{
			vBrushColor = vector(1.0f, 0.f, 0.f, 1.0f);
			vDiffuse += vBrushColor;
		}
	}
	else
	{
		if (In.vWorld.x > g_vBrushPos.x - g_fRadius &&
			In.vWorld.x < g_vBrushPos.x + g_fRadius &&
			In.vWorld.z > g_vBrushPos.z - g_fRadius &&
			In.vWorld.z < g_vBrushPos.z + g_fRadius)
		{
			vBrushColor = vector(0.0f, 0.f, 1.f, 1.0f);
			vDiffuse += vBrushColor;
		}
	}
	Out.vColor = vDiffuse;

	return Out;
}

technique11			DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
}

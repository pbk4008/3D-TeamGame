#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

texture2D g_DiffuseTexture;
sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_RECT_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float4 vRight : TEXCOORD1;
	float4 vUp : TEXCOORD2;
	float4 vLook : TEXCOORD3;
	float4 vTranslation : TEXCOORD4;
};
struct VS_RECT_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_RECT_OUT VS_RECT(VS_RECT_IN In)
{
	VS_RECT_OUT			Out = (VS_RECT_OUT)0;

	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), matInstance);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}
struct PS_RECT_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};
struct PS_RECT_OUT
{
	vector		vColor : SV_TARGET0;
};
PS_RECT_OUT PS_RECT(PS_RECT_IN In)
{
	PS_RECT_OUT Out = (PS_RECT_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.2f)
		discard;

	return Out;
}

technique11			DefaultTechnique
{
	pass Instancing_Rect
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_RECT();
	}
}
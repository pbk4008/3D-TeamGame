#include "Shader_RenderState.hpp"

/* 모든 전역변수들을 -> 상수테이블. */
/* 클라이언트로부터 값을 전달받아올 수 있다. */

cbuffer CameraDesc
{
	vector		g_vCamPosition;
};

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

Texture2D	g_DiffuseTexture;
uint g_iImageCountX; //가로줄수
uint g_iImageCountY; //세로줄수
uint g_iFrame; //전체장수

cbuffer weightbuffer
{
	float g_Weight;
};

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

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

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;

    return Out;
}

struct VS_OUT_NONALPHA
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD1;
};

VS_OUT_NONALPHA VS_MAIN_NONALPHA(VS_IN In)
{
	VS_OUT_NONALPHA Out = (VS_OUT_NONALPHA) 0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.wz = Out.vPosition.zw;

	return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
	//float4 weight : SV_Target1;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY);
	  
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    if (Out.vColor.a <= 0.f)
    {
        discard;
    }

	//Out.weight = float4(g_Weight.xxx, 1.f);
	
    return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY);

	half4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	Out.vColor = color;
	//Out.weight = color.r * g_Weight * Out.vColor;
	
    return Out;
}

struct PS_IN_NONALPHA
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD1;
};
struct PS_OUT_NONALPHA
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	float4 M : SV_Target3;
	float4 R : SV_Target4;
	float4 A : SV_Target5;
	float4 E : SV_Target6;
};
PS_OUT_NONALPHA PS_MAIN_NONALPHA(PS_IN_NONALPHA In)
{
	PS_OUT_NONALPHA Out = (PS_OUT_NONALPHA) 0;
	
	In.vUvDepth.x = (In.vUvDepth.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
	In.vUvDepth.y = (In.vUvDepth.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY);

	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1,1,1, 0);
	
	Out.M = float4(0, 0, 0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
	float4 color = float4(0.996f, 0.843f, 0.f, 1.f);
	float4 power = 0.03f;
	Out.E = color * 0.4f * diffuse;

	if (Out.diffuse.a <= 0.2f)
		discard;
	
	return Out;
}

technique11 DefaultTechnique
{
    pass AlphaDiscard
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
		//SetBlendState(BlendAble, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass AlphaBlend
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN1();
    }

	pass ShowNonAlpha
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_NONALPHA();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NONALPHA();

	}
}




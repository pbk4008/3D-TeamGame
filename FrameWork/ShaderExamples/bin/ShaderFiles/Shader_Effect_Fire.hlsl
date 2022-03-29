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

texture2D	g_DiffuseTexture;
uint g_iImageCountX; //가로줄수
uint g_iImageCountY; //세로줄수
uint g_iFrame; //전체장수

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

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY);
	  
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    if (Out.vColor.a < 0.2f)
    {
        discard;
    }

    return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY);

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

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

}




#include "Shader_RenderState.hpp"

//기본 텍스쳐의 색깔로 사용할때는 이 쉐이더 사용하면됨 
//주로 플레이어 UI나 직교UI들에 사용되고있음
cbuffer Matrices
{
    matrix g_WorldMatrix = (matrix) 0;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

float g_fX;
float g_fY;

texture2D g_DiffuseTexture;

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


/* 1. 정점의 스페이스 변환. */
/* 2. 픽셀에게 전달할 데이터를 결정한다. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	// VS_IN In : 정점버퍼에 정의되어있던 정점정보를 그대로 받아온것이다. 
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
		
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
	
    return Out;
}

/* SV_POSITION을 가진 데잍처에대해서만 원근투영.(정점의 w값으로 xyzw를 나눈다.) */
/* 뷰포트로 변환한다. */
/* 래스터라이즈.(둘러쌓여진 정점의 정보를 바탕으로 하여. 픽셀정보를 생성한다.) */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};


/* 1. 픽셀의 색을 결정한다. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vTexUV.x > g_fX)
    {
        discard;
    }
    
    if (In.vTexUV.y > g_fY)
    {
        discard;
    }
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    return Out;
}

technique11 DefaultTechnique
{
	/* 셰이더 기능의 캡슐화. */
	/* 조명연산(어둡게, 스펙큘러) + 그림자 + 노멀맵핑 */
    pass Normal
    {
		/* 렌더스테이츠에 대한 정의. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);


		/* 진입점함수를 지정한다. */
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
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
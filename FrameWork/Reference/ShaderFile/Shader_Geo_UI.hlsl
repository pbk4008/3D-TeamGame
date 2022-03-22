#include "Shader_RenderState.hpp"

/* 모든 전역변수들을 -> 상수테이블. */
/* 클라이언트로부터 값을 전달받아올 수 있다. */

cbuffer Matrices
{
    matrix g_WorldMatrix = (matrix) 0;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

texture2D g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;
};

/* 1. m_pDeviceContext->DrawIndexed() */
/* 2. 인덱스가 가지고 있던 인덱스 세개에 해당하는 정점 세개를 정점버퍼로부터 얻어온다. */
/* 3. VS_MAIN함수를 호출하면서 가져온 정점 세개중 하나씩 전달해준다.  */
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vPSize : PSIZE;

    row_major matrix TransformMatrix : WORLD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vPSize : PSIZE;
};


/* 1. 정점의 스페이스 변환. */
/* 2. 픽셀에게 전달할 데이터를 결정한다. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	// VS_IN In : 정점버퍼에 정의되어있던 정점정보를 그대로 받아온것이다. 
    matrix matWV, matWVP;
    
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
		
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    //vector vOffset = mul(vector(In.vOffset, 1.f), In.TransformMatrix);
    //Out.vOffset = mul(vOffset, g_WorldMatrix);
    
    //Out.vPSize.x = In.vPSize.x * In.TransformMatrix._11;
    //Out.vPSize.y = In.vPSize.y * In.TransformMatrix._22;
    Out.vPSize = In.vPSize;
	
    return Out;
}

struct GS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vPSize : PSIZE;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[6];

    vector vAxisY = vector(0.f, 1.f, 0.f, 0.f);

    vector vLook = normalize(float4(0.f, 0.f, 0.f, 1.f) - In[0].vPosition);
    vector vRight = vector(normalize(cross(vAxisY.xyz, vLook.xyz)), 0.f);
    vector vUp = vector(normalize(cross(vLook.xyz, vRight.xyz)), 0.f);
    
    Out[0].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * 0.5f) +
		(vUp * In[0].vPSize.y * 0.5f);
    Out[0].vTexUV = float2(0.f, 0.f);

    Out[1].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * -0.5f) +
		(vUp * In[0].vPSize.y * 0.5f);
    Out[1].vTexUV = float2(1.f, 0.f);

    Out[2].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * -0.5f) +
		(vUp * In[0].vPSize.y * -0.5f);
    Out[2].vTexUV = float2(1.f, 1.f);

    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();


    Out[3] = Out[0];

    Out[4] = Out[2];

    Out[5].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * 0.5f) +
		(vUp * In[0].vPSize.y * -0.5f);
    Out[5].vTexUV = float2(0.f, 1.f);

    OutStream.Append(Out[3]);
    OutStream.Append(Out[4]);
    OutStream.Append(Out[5]);
    OutStream.RestartStrip();
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


/* 1. 픽셀의 색을 결정한다. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

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
        SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* 진입점함수를 지정한다. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass AlphaBlend
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}

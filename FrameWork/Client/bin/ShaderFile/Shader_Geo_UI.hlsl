#include "Shader_RenderState.hpp"

/* ��� ������������ -> ������̺�. */
/* Ŭ���̾�Ʈ�κ��� ���� ���޹޾ƿ� �� �ִ�. */

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
/* 2. �ε����� ������ �ִ� �ε��� ������ �ش��ϴ� ���� ������ �������۷κ��� ���´�. */
/* 3. VS_MAIN�Լ��� ȣ���ϸ鼭 ������ ���� ������ �ϳ��� �������ش�.  */
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


/* 1. ������ �����̽� ��ȯ. */
/* 2. �ȼ����� ������ �����͸� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	// VS_IN In : �������ۿ� ���ǵǾ��ִ� ���������� �״�� �޾ƿ°��̴�. 
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


/* 1. �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    return Out;
}


technique11 DefaultTechnique
{
	/* ���̴� ����� ĸ��ȭ. */
	/* ������(��Ӱ�, ����ŧ��) + �׸��� + ��ָ��� */
    pass Normal
    {
		/* �������������� ���� ����. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* �������Լ��� �����Ѵ�. */
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

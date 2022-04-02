#include "Shader_RenderState.hpp"

/* ��� ������������ -> ������̺�. */
/* Ŭ���̾�Ʈ�κ��� ���� ���޹޾ƿ� �� �ִ�. */

cbuffer Matrices
{
    matrix g_WorldMatrix = (matrix) 0;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

float g_fX;
float g_fY;
float g_fAlpha;

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
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};


/* 1. ������ �����̽� ��ȯ. */
/* 2. �ȼ����� ������ �����͸� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	// VS_IN In : �������ۿ� ���ǵǾ��ִ� ���������� �״�� �޾ƿ°��̴�. 
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
		
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
	
    return Out;
}

/* SV_POSITION�� ���� ����ó�����ؼ��� ��������.(������ w������ xyzw�� ������.) */
/* ����Ʈ�� ��ȯ�Ѵ�. */
/* �����Ͷ�����.(�ѷ��׿��� ������ ������ �������� �Ͽ�. �ȼ������� �����Ѵ�.) */

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

    if (In.vTexUV.x > g_fX)
    {
        discard;
    }
    
    if (In.vTexUV.y > g_fY)
    {
        discard;
    }
    
    float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if(color.a == 0)
        discard;
    
    Out.vColor.rgb = color.rgb;
    Out.vColor.a = color.a * g_fAlpha;
    
    if (Out.vColor.a <= 0)
        discard;
    
    
    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vTexUV.y < 0.2f)
    {
        discard;
    }
    
    if (In.vTexUV.y > g_fY)
    {
        discard;
    }
    
    if (In.vTexUV.x > g_fX)
    {
        discard;
    }
    
    
    float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if (color.a == 0)
        discard;
    
    Out.vColor.rgb = color.rgb;
    Out.vColor.a = color.a * g_fAlpha;
    
    if (Out.vColor.a < 0.01)
        discard;
    
    Out.vColor.r = 1.f;
    Out.vColor.gb = 0.f;
    
    return Out;
}

PS_OUT PS_MAIN_YELLOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vTexUV.y < 0.2f)
    {
        discard;
    }
    
    if (In.vTexUV.y > g_fY)
    {
        discard;
    }
    
    if (In.vTexUV.x > g_fX)
    {
        discard;
    }
    
    
    float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if (color.a == 0)
        discard;
    
    Out.vColor.rgb = color.rgb;
    Out.vColor.a = color.a * g_fAlpha;
    
    if (Out.vColor.a < 0.01)
        discard;
    
    Out.vColor.r = 1.f;
    Out.vColor.g = 0.6f;
    Out.vColor.b = 0.2f;
    
    return Out;
}

PS_OUT PS_MAIN_GROGGY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (In.vTexUV.y < 0.2f)
    {
        discard;
    }
    
    if (In.vTexUV.y > g_fY)
    {
        discard;
    }
    
    if (In.vTexUV.x > g_fX)
    {
        discard;
    }
    
    
    float4 color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if (color.a == 0)
        discard;
    
    Out.vColor.rgb = color.rgb;
    Out.vColor.a = color.a * g_fAlpha;

    if (Out.vColor.a < 0.01)
        discard;
    
    Out.vColor.r = 1.f;
    Out.vColor.g = 0.8f;
    Out.vColor.b = 0.4f;
    
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


		/* �������Լ��� �����Ѵ�. */
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

    pass AlphaBlendRed
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RED();
    }

    pass AlphaBlendYellow
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_YELLOW();
    }

    pass AlphaBlendGroggy
    {
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GROGGY();
    }
}
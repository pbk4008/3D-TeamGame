#include "Shader_RenderState.hpp"

/* ��� ������������ -> ������̺�. */
/* Ŭ���̾�Ʈ�κ��� ���� ���޹޾ƿ� �� �ִ�. */

cbuffer CameraDesc
{
	vector		g_vCamPosition;
    float3		g_color;
};

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

texture2D	g_DiffuseTexture;
uint g_iImageCountX; //�����ټ�
uint g_iImageCountY; //�����ټ�
uint g_iFrame; //��ü���
float g_fLifeTime;
float g_fCurTime;


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
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	row_major matrix TransformMatrix : WORLD;
    float4		vTime : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
    float4		vTime : TEXCOORD0;
};


/* 1. ������ �����̽� ��ȯ. */
/* 2. �ȼ����� ������ �����͸� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	vector	vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vPSize.x = In.vPSize.x * In.TransformMatrix._11;
    Out.vPSize.y = In.vPSize.y * In.TransformMatrix._22;
    //Out.vPSize = In.vPSize;
    Out.vTime.x = In.vTime.x;
	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
    float4 vTime : TEXCOORD0;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};


[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[6];
	
    float Ratio = In[0].vTime.x / g_fLifeTime;
    In[0].vPSize.x = ((-In[0].vPSize.x) * Ratio + In[0].vPSize.x) / 2.f;
    In[0].vPSize.y = ((-In[0].vPSize.y) * Ratio + In[0].vPSize.y) / 2.f;

	vector		vAxisY = vector(0.f, 1.f, 0.f, 0.f);

	vector		vLook = normalize(g_vCamPosition - In[0].vPosition);
	vector		vRight = vector(normalize(cross(vAxisY.xyz, vLook.xyz)), 0.f);
	vector		vUp = vector(normalize(cross(vLook.xyz, vRight.xyz)), 0.f);

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * 0.5f) +
		(vUp * In[0].vPSize.y * 0.5f);
	Out[0].vTexUV = float2(0.f, 0.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);

	Out[1].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * -0.5f) +
		(vUp * In[0].vPSize.y * 0.5f);
	Out[1].vTexUV = float2(1.f, 0.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);

	Out[2].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * -0.5f) +
		(vUp * In[0].vPSize.y * -0.5f);
	Out[2].vTexUV = float2(1.f, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);

	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();


	Out[3] = Out[0];

	Out[4] = Out[2];

	Out[5].vPosition = In[0].vPosition + (vRight * In[0].vPSize.x * 0.5f) +
		(vUp * In[0].vPSize.y * -0.5f);
	Out[5].vTexUV = float2(0.f, 1.f);
	Out[5].vPosition = mul(Out[5].vPosition, matVP);

	OutStream.Append(Out[3]);
	OutStream.Append(Out[4]);
	OutStream.Append(Out[5]);
	OutStream.RestartStrip();
}

/* SV_POSITION�� ���� ����ó�����ؼ��� ��������.(������ w������ xyzw�� ������.) */
/* ����Ʈ�� ��ȯ�Ѵ�. */
/* �����Ͷ�����.(�ѷ��׿��� ������ ������ �������� �Ͽ�. �ȼ������� �����Ѵ�.) */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

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

/* 1. �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vColor.r = 1.f;
    Out.vColor.g = 0.6f;
    Out.vColor.b = 0.3f;

	if (Out.vColor.a < 0.01)
		discard;

	return Out;
}

PS_OUT PS_MAIN_MULTIIMAGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX); //���� �̹������� , ������ , 1������ �̹������� 
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountY) * (1.f / g_iImageCountY); //���� �̹������� , ������ , 1������ �̹�������

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    return Out;
}

PS_OUT_TEST PS_MAIN_TEST(PS_IN In)
{
	PS_OUT_TEST Out = (PS_OUT_TEST)0;

	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV.xy);	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vPosition.z / In.vPosition.w, In.vPosition.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1, 0);

	Out.M = float4(0, 0, 0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
    float4 color = float4(g_color, 1.f);
	float4 power = 1.0f;
	Out.E = color * power * diffuse;

	if (Out.diffuse.a < 0.1f)
		discard;
	
	return Out;
}


technique11			DefaultTechnique
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
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0  PS_MAIN();
	}

	pass AlphaBlend
	{
		/* �������������� ���� ����. */
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0  PS_MAIN();
	}

    pass AlphaBlendMultiImage
    {
		/* �������������� ���� ����. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_MULTIIMAGE();
    }

    pass AlphaAdd
    {
		/* �������������� ���� ����. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* �������Լ��� �����Ѵ�. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	pass Test
	{
		/* �������������� ���� ����. */
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);


		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_TEST();
	}
}





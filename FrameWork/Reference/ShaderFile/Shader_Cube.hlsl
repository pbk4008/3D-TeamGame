#include "Shader_RenderState.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

textureCUBE		g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;	
};


/* 1. ������ �����̽� ��ȯ. */
/* 2. �ȼ����� ������ �����͸� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	

	// VS_IN In : �������ۿ� ���ǵǾ��ִ� ���������� �״�� �޾ƿ°��̴�. 
	matrix			matWV, matWVP;	

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
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;	
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};


/* 1. �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	return Out;	
}

//D3D11_FILL_MODE FillMode;
//D3D11_CULL_MODE CullMode;
//BOOL FrontCounterClockwise;
//INT DepthBias;
//FLOAT DepthBiasClamp;
//FLOAT SlopeScaledDepthBias;
//BOOL DepthClipEnable;
//BOOL ScissorEnable;
//BOOL MultisampleEnable;
//BOOL AntialiasedLineEnable;

/* �ո�޸鿡 ���� ����. + �ĸ�ó�����⿡���� ����. */

//
//
///* �ȼ��� ������� */
//BlendState 
//{
//
//}
//
///* �����׽�Ʈ, ���ٽǿ� ���� ����. */
//DepthStencilState
//{
//
//}
//

technique11			DefaultTechnique
{
	/* ���̴� ����� ĸ��ȭ. */
	/* ������(��Ӱ�, ����ŧ��) + �׸��� + ��ָ��� */
	pass Normal
	{
		/* �������������� ���� ����. */		
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZWriteDisable, 0);

		/* �������Լ��� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
}





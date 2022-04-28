#include "Shader_RenderState.hpp"

/* ��� ������������ -> ������̺�. */
/* Ŭ���̾�Ʈ�κ��� ���� ���޹޾ƿ� �� �ִ�. */

cbuffer Matrices
{
	matrix		g_WorldMatrix  = (matrix)0;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
};

cbuffer fadeinout
{
	bool g_Fade = false;
	float g_Alpha = 0.f;
	float g_Weight = 0.f;
};
Texture2D		g_DiffuseTexture;
Texture2D		g_MaskTexture;

cbuffer Information
{
	uint g_iRow; // �����ټ�
	uint g_iColumn; // �����ټ�
	uint g_iIndex; // �ε���
	float g_fLifeTime;
	float g_fCurTime;
	float2 g_vUV;
};

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
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;	
};

struct VS_OUT_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
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

 VS_OUT_TRAIL VS_MAIN_TRAIL(VS_IN In)
{
	VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

	// VS_IN In : �������ۿ� ���ǵǾ��ִ� ���������� �״�� �޾ƿ°��̴�. 
	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	
	return Out;
}

 VS_OUT VS_MAIN_DAMAGEFONT(VS_IN In)
 {
	 VS_OUT			Out = (VS_OUT)0;

	 matrix			matWV, matWVP;

	 matWV = mul(g_WorldMatrix, g_ViewMatrix);
	 matWVP = mul(matWV, g_ProjMatrix);

	 Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	 //In.vTexUV.x = (In.vTexUV.x / g_iRow) + ((g_iIndex % g_iRow) * (1.f / g_iRow));
	 //In.vTexUV.y = (In.vTexUV.y / g_iColumn) + ((g_iIndex / g_iColumn) * (1.f / g_iColumn));
	 //// X
	 //switch (g_iIndex)
	 //{
	 //case 0:
	 //case 5:
		// if (0 == In.vTexUV.x)
		//	 In.vTexUV.x = 0.f;
		// else
		//	 In.vTexUV.x = 0.2f;
		// break;
	 //case 1:
	 //case 6:
		// if (0 == In.vTexUV.x)
		//	 In.vTexUV.x = 0.2f;
		// else
		//	 In.vTexUV.x = 0.4f;
		// break;
	 //case 2:
	 //case 7:
		// if (0 == In.vTexUV.x)
		//	 In.vTexUV.x = 0.4f;
		// else
		//	 In.vTexUV.x = 0.6f;
		// break;
	 //case 3:
	 //case 8:
		// if (0 == In.vTexUV.x)
		//	 In.vTexUV.x = 0.6f;
		// else
		//	 In.vTexUV.x = 0.8f;
		// break;
	 //case 4:
	 //case 9:
		// if (0 == In.vTexUV.x)
		//	 In.vTexUV.x = 0.8f;
		// else
		//	 In.vTexUV.x = 1.f;
		// break;
	 //}
	 //// Y
	 //switch (g_iIndex)
	 //{
	 //case 0:
	 //case 1:
	 //case 2:
	 //case 3:
	 //case 4:
		// if (0 == In.vTexUV.y)
		//	 In.vTexUV.y = 0.f;
		// else
		//	 In.vTexUV.y = 0.5f;
		// break;
	 //case 5:
	 //case 6:
	 //case 7:
	 //case 8:
	 //case 9:
		// if (0 == In.vTexUV.y)
		//	 In.vTexUV.y = 0.5f;
		// else
		//	 In.vTexUV.y = 1.f;
		// break;
	 //}
	 Out.vTexUV = In.vTexUV;
	 
	 return Out;
 }

VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = vector(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;

	return Out;
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


/* 1. �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_Fade == true)
	{
		Out.vColor.a *= g_Alpha;
	}
	//if (Out.vColor.a <= 0.1)
	//	discard;
	
	return Out;	
}

struct PS_OUT_Alpha
{
	half4		vColor : SV_TARGET0;
	half4		vWeight : SV_TARGET1;
};

PS_OUT_Alpha PS_MAIN_DAMAGEFONT_BG(PS_IN In)
{
	PS_OUT_Alpha		Out = (PS_OUT_Alpha)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;
	Out.vWeight = half4(g_Weight.xxx, 1.f);

	return Out;
}

PS_OUT_Alpha PS_MAIN_DAMAGEFONT(PS_IN In)
{
	PS_OUT_Alpha		Out = (PS_OUT_Alpha)0;

	vector vMaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	In.vTexUV.x = (In.vTexUV.x / g_iRow) + ((g_iIndex % g_iRow) * (1.f / g_iRow));
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor *= vMaskColor;
	Out.vColor.a *= g_Alpha;
	Out.vWeight = half4(g_Weight.xxx, 1.f);

	return Out;
}

struct PS_IN_TRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT_TRAIL
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
	float4 M : SV_Target3;
	float4 R : SV_Target4;
	float4 A : SV_Target5;
	float4 E : SV_Target6;
};

PS_OUT_TRAIL PS_MAIN_TRAIL(PS_IN_TRAIL In)
{
	PS_OUT_TRAIL Out = (PS_OUT_TRAIL) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse = diffuse;

	if (Out.diffuse.a == 0)
		discard;
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(1, 1, 1, 0);
	
	Out.M = float4(0, 0, 0, 1);
	Out.R = float4(1, 1, 1, 1);
	Out.A = float4(1, 1, 1, 1);
	float4 color = float4(0.99f, 0.43f, 0.2f, 0.5f);
	float4 power = 0.8f;
	Out.E = color * power;

	return Out;
}




technique11			DefaultTechnique
{
	/* ���̴� ����� ĸ��ȭ. */
	/* ������(��Ӱ�, ����ŧ��) + �׸��� + ��ָ��� */
	pass Normal
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 1
	pass Viewport
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 2
	pass Viewport2
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZBufferDisable, 0);
		SetBlendState(AlphaBlending2, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	// 3
	pass TrailRect
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}
	// 4
    pass AlphaBlend
    {
        SetRasterizerState(CullMode_None);
        SetDepthStencilState(ZBufferDisable, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
	// 5
	pass DamageFont
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZReadDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DAMAGEFONT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_DAMAGEFONT();
	}
	// 6
	pass Normal_ZreadDisable
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZReadDisable, 0);
		SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_DAMAGEFONT_BG();
	}
}

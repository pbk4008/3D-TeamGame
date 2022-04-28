#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

/* 모든 전역변수들을 -> 상수테이블. */
/* 클라이언트로부터 값을 전달받아올 수 있다. */


Texture2D	g_DiffuseTexture;

cbuffer Information
{
	uint g_iImageCountX; //가로줄수
	uint g_iImageCountY; //세로줄수
	uint g_iFrame; //인덱스
	float g_fLifeTime;
	float g_fCurTime;
};

/* 1. m_pDeviceContext->DrawIndexed() */
/* 2. 인덱스가 가지고 있던 인덱스 세개에 해당하는 정점 세개를 정점버퍼로부터 얻어온다. */
/* 3. VS_MAIN함수를 호출하면서 가져온 정점 세개중 하나씩 전달해준다.  */
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


/* 1. 정점의 스페이스 변환. */
/* 2. 픽셀에게 전달할 데이터를 결정한다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	vector	vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vPSize.x = In.vPSize.x * In.TransformMatrix._11;
    Out.vPSize.y = In.vPSize.y * In.TransformMatrix._22;
    Out.vTime.x = In.vTime.x;
	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
    float4		vTime : TEXCOORD0;
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

/* SV_POSITION을 가진 데잍처에대해서만 원근투영.(정점의 w값으로 xyzw를 나눈다.) */
/* 뷰포트로 변환한다. */
/* 래스터라이즈.(둘러쌓여진 정점의 정보를 바탕으로 하여. 픽셀정보를 생성한다.) */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
	float4		weight : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.r = g_color.r;
	Out.vColor.g = g_color.g;
	Out.vColor.b = g_color.b;

	if (Out.vColor.a < 0.01)
		discard;
	
	Out.weight = float4(g_Weight.xxx, 0.5f);

	return Out;
}

PS_OUT PS_MAIN_MULTIIMAGE_LEAF(PS_IN In) //낙엽에서씀
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX);
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountX) * (1.f / g_iImageCountY);
	
    float4 GreenAlpha = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.weight = float4(g_Weight.xxx, 0.5f);
	
    Out.vColor.r = 1.f; 
    Out.vColor.gb = 0.f;
	
    Out.vColor = Out.vColor * GreenAlpha.g;
	
    if (0.01f >= Out.vColor.a)
    {
        discard;
    }
	
    return Out;
}

struct PS_OUT_TEST
{
	half4 diffuse : SV_TARGET0;
	half4 normal : SV_TARGET1;
	half4 depth : SV_TARGET2;
	half4 mra : SV_Target3;
	half4 emission : SV_Target4;
};

PS_OUT_TEST PS_MAIN_TEST(PS_IN In)
{
	PS_OUT_TEST Out = (PS_OUT_TEST)0;
	
	In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX); //가로 이미지개수 , 프레임 , 1나누기 이미지개수 
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountX) * (1.f / g_iImageCountY); //세로 이미지개수 , 프레임 , 1나누기 이미지개수

	half Alpha = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).a;
	
	Out.diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);	
    Out.diffuse = Out.diffuse * Alpha;

	Out.depth = half4(In.vPosition.z / In.vPosition.w, In.vPosition.w / 300.f, 0.f, 0.f);
	Out.normal = half4(1, 1, 1, 0);

	Out.mra.r = 0.f;
	Out.mra.g = 1.f;
	Out.mra.b = 1.f;
	Out.mra.a = 1.f;
	
	Out.emission = g_color * g_empower;
	
	if (0.1f >= Out.diffuse.a)
		discard;
	
	return Out;
}

PS_OUT PS_MAIN_MULTIIMAGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    In.vTexUV.x = (In.vTexUV.x / g_iImageCountX) + (g_iFrame % g_iImageCountX) * (1.f / g_iImageCountX); //가로 이미지개수 , 프레임 , 1나누기 이미지개수 
    In.vTexUV.y = (In.vTexUV.y / g_iImageCountY) + (g_iFrame / g_iImageCountX) * (1.f / g_iImageCountY); //세로 이미지개수 , 프레임 , 1나누기 이미지개수
	
	float4 Color = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.weight = float4(g_Weight.xxx, 0.5f);
	
    Out.vColor = Out.vColor * Color.r;
	
    Out.weight = float4(g_Weight.xxx, 0.5f);
	
    return Out;
}
technique11			DefaultTechnique
{
	pass Normal //0
	{
		/* 렌더스테이츠에 대한 정의. */
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0  PS_MAIN();
	}

	pass AlphaBlend //1
	{
		/* 렌더스테이츠에 대한 정의. */
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0  PS_MAIN();
	}

    pass AlphaBlendMultiImageLeaf //2
    {
		/* 렌더스테이츠에 대한 정의. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_MULTIIMAGE_LEAF();
    }

    pass AlphaAdd //3
    {
		/* 렌더스테이츠에 대한 정의. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaAdditive, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	pass Test //4
	{
		/* 렌더스테이츠에 대한 정의. */
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_TEST();
	}

    pass AlphaBlendMultiImage //5
    {
		/* 렌더스테이츠에 대한 정의. */
        SetRasterizerState(CullMode_Default);
        SetDepthStencilState(ZDefault, 0);
        SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_MULTIIMAGE();
    }
}





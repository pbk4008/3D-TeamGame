#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D g_ShadowTexture;
Texture2D g_DiffuseTexture;
Texture2D g_BiNormalTexture;
Texture2D g_ORMTexture;

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	float3		vTangent	: TANGENT; 
	float3		vBiNormal	: BINORMAL;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float4 vTangent		: TANGENT;
	float4 vBiNormal	: BINORMAL;
	float4 vUvDepth		: TEXCOORD0;
	float  clip			: SV_ClipDistance0;
};

bool g_bUsingTool = false;

VS_OUT VS_MESH(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	Out.clip = dot(mul(vector(In.vPosition, 1.f), g_WorldMatrix), ClipPlane);

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
	float  clip : SV_ClipDistance0;
};

struct PS_OUT
{
	half4 diffuse	: SV_TARGET0;
	half4 normal	: SV_TARGET1;
	half4 depth		: SV_TARGET2;
	half4 mra		: SV_Target3;
	half4 emission	: SV_Target4;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3 orm = g_ORMTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	if (orm.b > 0.5f)
	{
		Out.diffuse = diffuse * half4(0.768f, 0.611f, 0.282f, 1.);
		Out.emission = half4(0.015f, 0.01f, 0.004f, 1);
	}
	else
	{
		Out.diffuse = diffuse;
		Out.emission = half4(0.0f, 0.0f, 0.0f, 1);
	}

	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = half4(normal, 0);
	
	half Metalic = orm.b + 0.2f;
	half Roughness = orm.g + 0.05f;
	half Ao = orm.r;
	
	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = Ao;
	Out.mra.a = 1.f;

	return Out;
}

technique11			DefaultTechnique
{
	pass DefaultShader
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MESH();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
	pass Default_Frame
	{
		SetRasterizerState(CullMode_Wireframe);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MESH();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN();
	}
}
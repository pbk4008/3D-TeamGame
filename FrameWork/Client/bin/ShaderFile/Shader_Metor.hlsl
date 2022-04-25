#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D g_ShadowTexture;
Texture2D g_DiffuseTexture;
Texture2D g_BiNormalTexture;

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
};
struct VS_OUT_VELOCITY
{
	float4 vPosition : SV_POSITION;
	float4 vVelocity : TEXCOORD0;
};

bool g_bUsingTool = false;

VS_OUT VS_MESH(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	half4 postion = half4(In.vPosition, 1.f);

	Out.vPosition = mul(postion, matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	
	return Out;
}
VS_OUT_VELOCITY VS_MAIN_VELOCITY(VS_IN In)
{
	VS_OUT_VELOCITY Out = (VS_OUT_VELOCITY)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);


	half4 vCurPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vPosition = vCurPosition;
	half4 normal = mul(vector(In.vNormal, 0.f), matWVP);
	normal = mul(normal, matWVP);

	half4 curpos = Out.vPosition;
	half4 prepos = mul(vector(In.vPosition, 1.f), g_PreWorldViewProj);
	half3 dir = curpos.xyz - prepos.xyz;

	half a = dot(normalize(dir), normalize(normal.xyz));
	if (a < 0.f)
		Out.vPosition = prepos;
	else
		Out.vPosition = curpos;

	half2 velocity = (curpos.xy / curpos.w) - (prepos.xy / prepos.w);
	Out.vVelocity.xy = velocity * 0.5f;
	Out.vVelocity.y *= -1.f;
	Out.vVelocity.z = Out.vPosition.z;
	Out.vVelocity.w = Out.vPosition.w;

	return Out;
}

// VS_SHADOW_MAP
//*---------------------------------------------------------------------------------------------*

//*---------------------------------------------------------------------------------------------*

struct PS_IN
{
	half4 vPosition		: SV_POSITION;
	half4 vNormal		: NORMAL;
	half4 vTangent		: TANGENT;
	half4 vBiNormal		: BINORMAL;
	half4 vUvDepth		: TEXCOORD0;
};
struct PS_IN_VELOCITY
{
	float4 vPosition : SV_POSITION;
	float4 vVelocity : TEXCOORD0;
};
struct PS_OUT
{
	half4 diffuse	: SV_TARGET0;
	half4 normal	: SV_TARGET1;
	half4 depth		: SV_TARGET2;
	half4 mra		: SV_Target3;
	half4 emission	: SV_Target4;
};
struct PS_OUT_VELOCITY
{
	vector VelocityMap : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = half4(normal, 0);
	
	half Metalic = 0.f;
	half Roughness = 0.9;
	half Ao = 1.f;

	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = Ao;
	Out.mra.a = 1.f;
	
	half4 tmp = half4(Out.normal.rgb * 2.f - 1.f, 0.f);
	float4 rim = RimLighting(tmp, g_camdir, g_rimintensity, g_rimcolor);
	Out.emission = g_color * g_empower;
	Out.emission += rim;

	return Out;
}
PS_OUT_VELOCITY PS_MAIN_VELOCITY(PS_IN_VELOCITY In)
{
	PS_OUT_VELOCITY Out = (PS_OUT_VELOCITY)0.f;

	Out.VelocityMap.xy = In.vVelocity.xy;
	Out.VelocityMap.z = In.vVelocity.z / In.vVelocity.w;
	Out.VelocityMap.w = In.vVelocity.w / 300.f;

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
	pass VelocityMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VELOCITY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VELOCITY();
	}
}
#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D g_DiffuseTexture;
Texture2D g_ShadowTexture;
Texture2D g_BiNormalTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT; 
	float3		vBiNormal : BINORMAL;
	
	float4 vRight : TEXCOORD1;
	float4 vUp  : TEXCOORD2;
	float4 vLook  : TEXCOORD3;
	float4 vTranslation  : TEXCOORD4;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float4 vTangent		: TANGENT;
	float4 vBiNormal	: BINORMAL;
	float4 vUvDepth		: TEXCOORD0;
	float clip			: SV_ClipDistance0;
};

bool g_bUsingTool = false;

VS_OUT VS_MESH(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	
	if (g_bUsingTool)
	{
		matInstance = float4x4(	1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
	}

	vector vPosition = mul(vector(In.vPosition, 1.f), matInstance);
	
	vector vNormal = mul(vector(In.vNormal, 0.f), matInstance);
	vector vBiNormal = mul(vector(In.vBiNormal, 0.f), matInstance);
	vector vTangent = mul(vector(In.vTangent, 0.f), matInstance);
	
	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vBiNormal, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTangent, g_WorldMatrix));
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;
	
	Out.clip = dot(mul(vPosition, g_WorldMatrix), ClipPlane);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*
// VS_Velocity
struct VS_OUT_VELOCITY
{
	float4 vPosition : SV_POSITION;
	float4 vVelocity : TEXCOORD0;
};

VS_OUT_VELOCITY VS_MAIN_VELOCITY(VS_IN In)
{
	VS_OUT_VELOCITY Out = (VS_OUT_VELOCITY) 0;
	
	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	
	if (g_bUsingTool)
	{
		matInstance = float4x4(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
	}

	half4 vPosition = mul(vector(In.vPosition, 1.f), matInstance);
	half4 vNormal = mul(vector(In.vNormal, 0.f), matInstance);
	
	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	half4 vCurPosition = mul(vPosition, matWVP);

	Out.vPosition = vCurPosition;
	vNormal = mul(vNormal, matWVP);
	half4 curpos = Out.vPosition;
	half4 prepos = mul(vPosition, g_PreWorldViewProj);
	half3 dir = curpos.xyz - prepos.xyz;
	
	half a = dot(normalize(dir), normalize(vNormal.xyz));
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

// PS Velocity Map
struct PS_IN_VELOCITY
{
	float4 vPosition : SV_POSITION;
	float4 vVelocity : TEXCOORD0;
};

struct PS_OUT_VELOCITY
{
	vector VelocityMap : SV_TARGET0;
};

PS_OUT_VELOCITY PS_MAIN_VELOCITY(PS_IN_VELOCITY In)
{
	PS_OUT_VELOCITY Out = (PS_OUT_VELOCITY) 0.f;
	
	Out.VelocityMap.xy = In.vVelocity.xy;
	Out.VelocityMap.z = In.vVelocity.z / In.vVelocity.w;
	Out.VelocityMap.w = In.vVelocity.w / 300.f;
	
	return Out;
}

//*---------------------------------------------------------------------------------------------*

//*---------------------------------------------------------------------------------------------*
// VS_SHADOW_MAP
struct VS_OUT_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
	float3 worldpos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

	matrix matInstance = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), matInstance);

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vClipPos = Out.vPosition;
	
	half4 worldpos = mul(vPosition, g_WorldMatrix);
	Out.worldpos = worldpos.xyz;
	
	return Out;
}

// PS SHADOW_MAP
struct PS_IN_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
	float3 worldpos : TEXCOORD2;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	const half OneDividzFar = 1 / 300.f;
	half4 color = 1;
	color.xyz = length(In.worldpos - g_LightPos) * OneDividzFar;
	Out.vShadowDepthMap = color;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBiNormal : BINORMAL;
	float4 vUvDepth : TEXCOORD0;
	float clip : SV_ClipDistance0;
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
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = half4(normal, 0);
	
	Out.mra.r = 0.4f;
	Out.mra.g = 0.65f;
	Out.mra.b = 1.f;
	Out.mra.a = 1.f;
	Out.emission = half4(0, 0, 0, 1);
	
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
	pass ShadowMap
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
	pass VelocityMap
	{
		SetRasterizerState(CullMode_None);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VELOCITY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VELOCITY();
	}
}
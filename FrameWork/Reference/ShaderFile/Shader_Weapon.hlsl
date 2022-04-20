#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D	g_ShadowTexture;

Texture2D	g_DiffuseTexture;
Texture2D	g_BiNormalTexture;

Texture2D	g_MRATexture;
Texture2D	g_CEOTexture;
Texture2D	g_DissolveTex;

Texture2D	g_GradientTex;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	float3	vBiNormal : BINORMAL;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float4	vBiNormal	: BINORMAL;
	float4	vUvDepth	: TEXCOORD0;
};

VS_OUT VS_MAIN_ANIM(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	


	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
	
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));

	Out.vUvDepth.zw = Out.vPosition.zw;
	
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
	
	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	
	half4 vCurPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vPosition = vCurPosition;
	half4 normal = mul(vector(In.vNormal, 0.f),matWVP);
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
// VS_SHADOWMAP

struct VS_OUT_SHADOW
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
	float4 vClipPos : TEXCOORD1;
	float3 worldpos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0.f;
	
	matrix matWV,matWVP;
	
	vector vPosition = vector(In.vPosition, 1.f);
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vClipPos = Out.vPosition;
	
	float4 worldpos = mul(vPosition, g_WorldMatrix);
	Out.worldpos = worldpos.xyz;
	return Out;
}

// PS SHADOWMAP
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
	
	const float OneDividzFar = 1 / 300.f;
	float4 color = 1;
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
};

struct PS_OUT
{
	half4 diffuse	: SV_TARGET0;
	half4 normal	: SV_TARGET1;
	half4 depth		: SV_TARGET2;
	half4 mra		: SV_Target3;
	half4 emission	: SV_Target4;
	half4 rimlight	: SV_Target5;
};

PS_OUT PS_MAIN_WEAPONE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(normal, 0);
	
	float Metalic = mra.r + g_Metalic;	
	float Roughness = mra.g + g_Roughness;
	float AO = mra.b + g_AO;
	
	float4 color = float4(1.f, 0.8431f, 0.f, 1.f);
	float4 power = 0.8f;

	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = AO;
	Out.mra.a = 1.f;
	
	Out.emission = g_color * power * ceo.g;
	
	if (g_rimlightcheck == true)
	{
		half4 normal = half4(Out.normal.rgb * 2.f - 1.f, 0.f);
		float4 rim = RimLighting(normal, g_camdir, g_rimintensity, g_rimcolor);
		Out.emission += rim;
	}
	
	return Out;	
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	float4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	float3 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	float4 dissolve = g_DissolveTex.Sample(DefaultSampler, In.vUvDepth.xy);
	
	if (dissolve.r - g_dissolvetime <= 0)
		discard;
	
	float3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;

	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	Out.normal = float4(normal, 0);
	
	float Metalic = mra.r + g_Metalic;
	float Roughness = mra.g + g_Roughness;
	float AO = mra.b + g_AO;
	
	float4 color = float4(1.f, 0.8431f, 0.f, 1.f);
	float4 power = 0.8f;

	Out.mra.r = Metalic;
	Out.mra.g = Roughness;
	Out.mra.b = AO;
	Out.mra.a = 1.f;
	
	Out.emission = g_color * power * ceo.g;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*
// VS Motion Trail
struct VS_OUT_MOTIONTRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vUvDepth : TEXCOORD0;
};

VS_OUT_MOTIONTRAIL VS_MAIN_MOTIONTRAIL(VS_IN In)
{
	VS_OUT_MOTIONTRAIL Out = (VS_OUT_MOTIONTRAIL) 0;
	
	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;

	return Out;
}
// PS Motion Trail
struct PS_IN_MOTIONTRAIL
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vUvDepth : TEXCOORD0;
};

struct PS_OUT_MOTIONTRAIL
{
	half4 Motiontrail : SV_TARGET0;
};

PS_OUT_MOTIONTRAIL PS_MAIN_MOTIONTRAIL(PS_IN_MOTIONTRAIL In)
{
	PS_OUT_MOTIONTRAIL Out = (PS_OUT_MOTIONTRAIL) 0;

	half3 color = g_GradientTex.Sample(DefaultSampler, half2(g_UVdvid, 0)).rgb;
	half4 normal = half4(In.vNormal.xyz, 0.f);
	Out.Motiontrail = MotionTrailRim(normal, g_camdir, g_rimintensity, color);
	Out.Motiontrail.a *= g_Fade;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*
technique11			DefaultTechnique
{	
	pass Weapone //------------------------------------------------------------------------------------0 normal
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WEAPONE();
	}

	pass WeaponeDissolve //------------------------------------------------------------------------------------1 dissolve
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
	}

	pass ShadowMap //-----------------------------------------------------------------------------------------2 ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass VelocityMap //-----------------------------------------------------------------------------------------4 MotionTrail
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_VELOCITY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VELOCITY();
	}

	pass MotionTrail //-----------------------------------------------------------------------------------------3 VeloCityMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_MOTIONTRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MOTIONTRAIL();
	}
}





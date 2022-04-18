#include "Shader_RenderState.hpp"
#include "Shader_Share.hlsli"
#include "Shader_ShareFuntion.hlsli"

Texture2D	g_DiffuseTexture;
Texture2D	g_BiNormalTexture;
Texture2D	g_MRATexture;
Texture2D	g_OMERTexture;
Texture2D	g_CEOTexture;
Texture2D	g_OtherTexture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
	float3	vBiNormal : BINORMAL;
	uint4	vBlendIndex : BLENDINDEX;
	float4	vBlendWeight : BLENDWEIGHT;
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

	matrix			matWV, matWVP;	

	float		fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
		
	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vPosition = mul(vPosition, matWVP);
	Out.vPosition = vPosition;

	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector		vBiNoraml = mul(vector(In.vBiNormal, 0.f), BoneMatrix);
	vector		vTantent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vBiNormal = normalize(mul(vBiNoraml, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTantent, g_WorldMatrix));
	Out.vUvDepth.xy = In.vTexUV.xy;
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

	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);
	
	matrix OldBoneMatrix =  mul(g_OldBoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
							mul(g_OldBoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
							mul(g_OldBoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
							mul(g_OldBoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	
	half4 vCurPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	half4 vPrePrositon = mul(vector(In.vPosition, 1.f), OldBoneMatrix);
	
	vCurPosition = mul(vCurPosition, matWVP);
	Out.vPosition = vCurPosition;
	half4 normal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	normal = mul(normal, matWVP);

	half4 curpos = Out.vPosition;
	half4 prepos = mul(vPrePrositon, g_PreWorldViewProj);
	half3 dir = curpos.xyz - prepos.xyz;
	
	half a = dot(normalize(dir), normalize(normal.xyz));
	if(a < 0.f)
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
	
	half fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	matrix BoneMatrix = mul(g_BoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_BoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);
	
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	
	matWV = mul(g_WorldMatrix, g_LightView);
	matWVP = mul(matWV, g_LightProj);
	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vClipPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	
	half4 worldpos = mul(vPosition, g_WorldMatrix);
	Out.worldpos = worldpos.xyz;
	
	return Out;
}

// PS SHADOWMAP
struct PS_IN_SHADOW
{
	half4 vPosition : SV_Position;
	half2 vTexUV : TEXCOORD0;
	half4 vClipPos : TEXCOORD1;
	half3 worldpos : TEXCOORD2;
};

struct PS_OUT_SHADOW
{
	vector vShadowDepthMap : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0.f;
	
	float4 color = 1;
	float OneDividzFar = 1 / 300.f;
	color.xyz = length(In.worldpos - g_LightPos) * OneDividzFar;
	Out.vShadowDepthMap = color;
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float4 vTangent		: TANGENT;
	float4 vBiNormal	: BINORMAL;
	float4 vUvDepth		: TEXCOORD0;
};

struct PS_OUT
{
	half4 diffuse	: SV_TARGET0;
	half4 normal	: SV_TARGET1;
	half4 depth		: SV_TARGET2;
	half4 mra		: SV_Target3;
	half4 emission	: SV_Target4;
};

PS_OUT PS_MAIN_TOP(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);

	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.depth = float4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.5f, 0.f);
	
	half accvalue = diffuse.r + diffuse.g - diffuse.b;
	if(accvalue > 0.6f)
	{
		Out.diffuse = half4(0.9f, 0.57f, 0.f, 1.f) * accvalue;
		Out.normal = half4(normal, 0.f);
		half metalic = omer.g * 0.05f + mra.r;
		half roughness = omer.a + mra.g * 3.3f - mra.r - mra.b - (1 - omer.g);
		half ao = mra.b;
		Out.mra.r = metalic + g_Metalic;
		Out.mra.g = roughness + g_Roughness;
		Out.mra.b = ao + g_AO;
		Out.mra.a = 1.f;
		Out.emission = g_color * g_empower * omer.b;
	}
	else
	{	
		Out.normal = half4(normal, 0.f);
		Out.diffuse = diffuse;
		
		half metalic = omer.g * 0.05f + mra.r;
		half roughness = omer.a + mra.g * 3.f - mra.r - mra.b - (1 - omer.g);
		half ao = mra.b;
		Out.mra.r = metalic + g_Metalic;
		Out.mra.g = roughness + g_Roughness;
		Out.mra.b = ao + g_AO;
		Out.mra.a = 1.f;
		Out.emission = g_color * g_empower * omer.b;

	}
	
	if(g_rimlightcheck == true)
	{
		half4 normal = half4(In.vNormal.xyz, 0.f);
		float4 rim = RimLighting(normal, g_camdir, g_rimintensity, g_rimcolor);
		Out.emission += rim;
	}
	
	return Out;	
}

PS_OUT PS_MAIN_DOWN(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 mra = g_MRATexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 ceo = g_CEOTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	Out.normal = half4(normal, 0.f);
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	half metalic = mra.r;
	half roughness = 0.5f + mra.g * 3.f - mra.r * 2;
	half ao = ceo.b * 1.f * mra.b;
	half4 E = ceo.g * g_color * g_empower;
	
	Out.mra.r = metalic + g_Metalic;
	Out.mra.g = roughness + g_Roughness;
	Out.mra.b = ao + g_AO;
	Out.mra.a = 1.f;
	Out.emission = E;
	
	if (g_rimlightcheck == true)
	{
		half4 normal = half4(In.vNormal.xyz, 0.f);
		float4 rim = RimLighting(normal, g_camdir, g_rimintensity, g_rimcolor);
		Out.emission += rim;
	}
	
	return Out;
}

PS_OUT PS_MAIN_CLOAK(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.diffuse = diffuse;
	Out.normal = half4(normal, 0.f);
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	Out.mra.r = omer.g + g_Metalic;
	Out.mra.g = omer.a + g_Roughness;
	Out.mra.b = omer.r + g_AO;
	Out.mra.a =  1.f;
	Out.emission = half4(omer.b, omer.b, omer.b, 1) * g_color + g_empower;
	
	return Out;
}

PS_OUT PS_MAIN_HAIR(PS_IN In)
{
	PS_OUT Out = (PS_OUT) 0;
	half4 diffuse = g_OtherTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	half4 diffuse2 = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	diffuse.rgb = diffuse.rgb;
	half4 omer = g_OMERTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	Out.diffuse.xyz = diffuse * 0.5f + 0.5f;
	Out.diffuse.w = diffuse2.w;
	
	half3 normal = g_BiNormalTexture.Sample(DefaultSampler, In.vUvDepth.xy).xyz;
	half3x3 tbn = { In.vTangent.xyz, In.vBiNormal.xyz, In.vNormal.xyz };
	normal = Normalmapping(normal, tbn);
	
	Out.normal = half4(normal, 0.f);
	Out.depth = half4(In.vUvDepth.z / In.vUvDepth.w, In.vUvDepth.w / 300.f, 0.f, 0.f);
	
	Out.mra.r = omer.g + g_Metalic;
	Out.mra.g = omer.a + g_Roughness;
	Out.mra.b = omer.r + g_AO;
	Out.mra.a = 1.f;
	Out.emission = half4(omer.b, omer.b, omer.b, 1) * g_color + g_empower;
	
	return Out;
}

//*---------------------------------------------------------------------------------------------*
struct VS_OUT_MOTIONTRAIL
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float4 vUvDepth		: TEXCOORD0;
};

VS_OUT_MOTIONTRAIL VS_MAIN_MOTIONTRAIL(VS_IN In)
{
	VS_OUT_MOTIONTRAIL Out = (VS_OUT_MOTIONTRAIL) 0;

	matrix matWV, matWVP;

	float fWeightw = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix BoneMatrix = mul(g_TrailBoneMatrices.Bone[In.vBlendIndex.x], In.vBlendWeight.x) +
						mul(g_TrailBoneMatrices.Bone[In.vBlendIndex.y], In.vBlendWeight.y) +
						mul(g_TrailBoneMatrices.Bone[In.vBlendIndex.z], In.vBlendWeight.z) +
						mul(g_TrailBoneMatrices.Bone[In.vBlendIndex.w], In.vBlendWeight.w);

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
		
	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vPosition = mul(vPosition, matWVP);
	Out.vPosition = vPosition;

	vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vUvDepth.xy = In.vTexUV.xy;
	Out.vUvDepth.zw = Out.vPosition.zw;

	return Out;
}
struct PS_IN_MOTIONTRAIL
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float4 vUvDepth		: TEXCOORD0;
};

struct PS_OUT_MOTIONTRAIL
{
	half4 Motiontrail : SV_TARGET0;
};

PS_OUT_MOTIONTRAIL PS_MAIN_MOTIONTRAIL(PS_IN_MOTIONTRAIL In)
{
	PS_OUT_MOTIONTRAIL Out = (PS_OUT_MOTIONTRAIL) 0;

	half4 diffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vUvDepth.xy);
	
	half4 normal = half4(In.vNormal.xyz, 0.f);
	Out.Motiontrail += RimLighting(normal, g_camdir, g_rimintensity, g_rimcolor);
	
	return Out;
}
//*---------------------------------------------------------------------------------------------*

technique11			DefaultTechnique
{	
	pass SliverManeTop //------------------------------------------------------------------------------------0 Top
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0  PS_MAIN_TOP();
	}

	pass SliverManeDown //------------------------------------------------------------------------------------1 Down
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOWN();
	}

	pass SliverManeCloak //------------------------------------------------------------------------------------2 Cloak
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOAK();
	}

	pass SliverManeHair //------------------------------------------------------------------------------------3 Hair
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HAIR();
	}

	pass Motiontrail //-----------------------------------------------------------------------------------------4 Motion Trail
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_MOTIONTRAIL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MOTIONTRAIL();
	}

	pass Velocity //-----------------------------------------------------------------------------------------5 Anim Velocity
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_VELOCITY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_VELOCITY();
	}

	pass ShadowANIM //-----------------------------------------------------------------------------------------6 Anim ShadowMap
	{
		SetRasterizerState(CullMode_Default);
		SetDepthStencilState(ZDefault, 0);
		SetBlendState(BlendDisable, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 진입점함수를 지정한다. */
		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
}





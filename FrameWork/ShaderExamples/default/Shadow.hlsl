#include "Constants.hlsl"


cbuffer Matrices
{
	matrix g_WorldMatrix = (matrix) 0;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
	matrix g_LightViewMatrix;
	matrix g_LightProjMatrix;
};

cbuffer LightBuffer2
{
	float3	lightposition;
	float	padding;
}

struct Vs_In
{
	float3 position : POSITIONT;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct Vs_Out
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 ProjPosition : TEXCOORD1;
	float4 ClipPosition : TEXCOORD2;
};

Vs_Out Vs_Main(Vs_In In)
{
	Vs_Out Out = (Vs_Out) 0;
	
	matrix matWV, matWVP;
	
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	
	matWVP = mul(matWV, g_ProjMatrix);
	
	Out.position = mul(vector(In.position, 1.f), matWVP);
	Out.tex = In.tex;
	
	Out.ProjPosition = Out.position;
	
	Out.ClipPosition = mul(vector(In.position, 1.f), g_WorldMatrix);
	Out.ClipPosition = mul(Out.ClipPosition, g_LightViewMatrix);
	Out.ClipPosition = mul(Out.ClipPosition, g_LightProjMatrix);
	
	return Out;
	
}

struct Ps_In
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 ProjPosition : TEXCOORD1;
	float4 ClipPosition : TEXCOORD2;
};

struct Ps_Out
{
	vector Diffuse : SV_TARGET0;
	vector Normal : SV_TARGET1;
	vector Depth : SV_TARGET2;
};

Ps_Out Ps_Main(Ps_In In)
{
	float depth = In.ClipPosition.z / In.ClipPosition.w;
	
	
}
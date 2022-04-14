sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler ClampSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

struct BoneMatrixArray
{
	matrix Bone[256];
};

cbuffer BoneMatricesBuffer
{
	BoneMatrixArray g_BoneMatrices;
	BoneMatrixArray g_OldBoneMatrices;
};

cbuffer Matrices
{
	matrix g_WorldMatrix = (matrix) 0;
	matrix g_ViewMatrix = (matrix) 0;
	matrix g_ProjMatrix = (matrix) 0;
	vector g_CamPos = (vector) 0;
};

cbuffer LightBuffer
{
	matrix g_LightView = (matrix) 0;
	matrix g_LightProj = (matrix) 0;
	float3 g_LightPos = (float3) 0;
};

cbuffer PRTMaterialBuffer
{
	float g_Metalic = (float) 0;
	float g_Roughness = (float) 0;
	float g_AO = (float) 0;
};

cbuffer Colorbuffer
{
	float4 g_color = (float4) 0;
	float g_empower = (float) 0;
};

cbuffer ClipPlaneBuffer
{
	float4 ClipPlane = (float4)0;
};

cbuffer Camerabuffer
{
	vector g_vCamPosition = (vector)0;
	float g_Weight = (float)0;
};

cbuffer Dissolvebuffer
{
	float	g_dissolvetime = (float)0;
	bool	g_bdissolve = (bool) 0;
};

cbuffer Limlight
{
	bool	g_rimlightcheck = (bool)0;
	float	g_rimintensity = (float)0;
	float4	g_rimcolor = (float4)0;
	float4	g_camdir = (float4)0;
};

cbuffer Motionblur
{
	matrix g_RotationMat = (matrix) 0;
	matrix g_PreWorldViewProj = (matrix) 0;
};
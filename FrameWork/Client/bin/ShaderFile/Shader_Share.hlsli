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
	float4 ClipPlane;
};

cbuffer Camerabuffer
{
	vector g_vCamPosition;
	float g_Weight;
};

cbuffer Dissolvebuffer
{
	float	g_dissolvetime;
	bool	g_bdissolve;
};
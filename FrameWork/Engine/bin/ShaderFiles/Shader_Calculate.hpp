float PI = 3.141592f;

//----------------------------------BelndState Setup-----------------------------------//
BlendState BlendDisable
{
	BlendEnable[0] = false;
};

BlendState OneBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = One;
	DestBlend = One;
	BlendOp = Add;
};

//----------------------------------AlphaBlending Setup-----------------------------------//
BlendState AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

//----------------------------------CullMode Setup-----------------------------------//
RasterizerState CullMode_Default
{
	FillMode = solid;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

//----------------------------------ZTest Setup-----------------------------------//
DepthStencilState ZDefault
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};

DepthStencilState ZTestDiable
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

//----------------------------------Calculate Function-----------------------------//

float3 FresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0f - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0f);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
	return F0 + (max(float(1.f - roughness).xxx, F0) - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

float NormalDistributionGGXTR(float3 N, float3 H, float A)
{
	float a = A * A;
	float a2 = a * a;
	float NdotH = saturate(dot(N, H));
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f;

	float num = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return num / denom;
}

float GeometrySmit(float3 N, float3 V, float3 L, float K)
{
	float NdotV = saturate(dot(N, V));
	float NdotL = saturate(dot(N, L));

	float ggx2 = GeometrySchlickGGX(NdotV, K);
	float ggx1 = GeometrySchlickGGX(NdotL, K);

	return ggx1 * ggx2;
}

int g_blurSize = 3;

float pixelKernel[13] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
};

float blurWeights[13] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};
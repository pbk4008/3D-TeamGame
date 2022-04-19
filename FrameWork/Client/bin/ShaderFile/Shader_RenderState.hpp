
float PI = 3.141592f;

/*
//D3D11_FILL_MODE FillMode;
//D3D11_CULL_MODE CullMode;
//BOOL FrontCounterClockwise;
//INT DepthBias;
//FLOAT DepthBiasClamp;
//FLOAT SlopeScaledDepthBias;
//BOOL DepthClipEnable;
//BOOL ScissorEnable;
//BOOL MultisampleEnable;
//BOOL AntialiasedLineEnable;
*/

RasterizerState CullMode_Default
{
	FillMode = solid;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_Wireframe
{
	FillMode = wireframe;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_None
{
	CullMode = None;
};


//BOOL DepthEnable;
//D3D11_DEPTH_WRITE_MASK DepthWriteMask;
//D3D11_COMPARISON_FUNC DepthFunc;
//BOOL StencilEnable;
//UINT8 StencilReadMask;
//UINT8 StencilWriteMask;
//D3D11_DEPTH_STENCILOP_DESC FrontFace;
//D3D11_DEPTH_STENCILOP_DESC BackFace;

DepthStencilState ZDefault
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};

DepthStencilState ZWriteDisable
{
	DepthEnable = false;
	DepthWriteMask = Zero;
	DepthFunc = less;
};

DepthStencilState ZBufferDisable
{
	DepthEnable = true;
	DepthWriteMask = Zero;
	DepthFunc = less;
};

DepthStencilState ZAlphaBlend
{
	DepthEnable = true;
	StencilEnable = true;
	DepthWriteMask = Zero;
	DepthFunc = less;
	StencilReadMask = 0xff;
	StencilWriteMask = 0xff;
};

//BOOL BlendEnable;
//D3D11_BLEND SrcBlend;
//D3D11_BLEND DestBlend;
//D3D11_BLEND_OP BlendOp;
//D3D11_BLEND SrcBlendAlpha;
//D3D11_BLEND DestBlendAlpha;
//D3D11_BLEND_OP BlendOpAlpha;
//UINT8 RenderTargetWriteMask;

BlendState AlphaBlending2
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendOp = Add;

	SrcBlend = Src_Alpha;
	DestBlend = One;

	SrcBlendAlpha = Zero;
	DestBlendAlpha = Zero;
	BlendOpAlpha = Add;
};

BlendState AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendOp = Add;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;

	SrcBlendAlpha = Zero;
	DestBlendAlpha = Zero;
	BlendOpAlpha = Add;
};

BlendState AlphaAdditive
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = One;
	BlendOp = Add;

	SrcBlendAlpha = One;
	DestBlendAlpha = One;
	BlendOpAlpha = Add;

	RenderTargetWriteMask[0] = 0x0F;
	RenderTargetWriteMask[1] = 0x0F;

};

BlendState OneBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = One;
	DestBlend = One;
	BlendOp = Add;
};


BlendState BlendDisable
{
	BlendEnable[0] = false;
};

BlendState BlendAlphaFire
{
	BlendEnable[0] = true;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;

	SrcBlendAlpha = One;
	DestBlendAlpha = Zero;
	BlendOpAlpha = Add;
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
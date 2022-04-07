
static const uint SSCS_MAX_STEPS = 16;

DepthStencilState TwoSidedStencil
{
	DepthEnable = true;
	DepthWriteMask = zero;
	DepthFunc = less;

	StencilEnable = true;
	StencilReadMask = 0xFFFFFFFF;
	StencilWriteMask = 0xFFFFFFFF;

	FrontFaceStencilFunc = Always;
	FrontFaceStencilDepthFail = Decr;
	FrontFaceStencilPass = Keep;
	FrontFaceStencilFail = Keep;

	BackFaceStencilFunc = Always;
	BackFaceStencilDepthFail = Incr;
	BackFaceStencilPass = Keep;
	BackFaceStencilFail = Keep;
};

DepthStencilState SidedStencil
{
	DepthEnable = false;
	DepthWriteMask = all;
	DepthFunc = not_equal;

	StencilEnable = false;
	StencilReadMask = 0xFFFFFFFF;
	StencilWriteMask = 0xFFFFFFFF;

	FrontFaceStencilFunc = not_equal;
	FrontFaceStencilDepthFail = Decr;
	FrontFaceStencilPass = Keep;
	FrontFaceStencilFail = Keep;

	BackFaceStencilFunc = not_equal;
	BackFaceStencilDepthFail = Incr;
	BackFaceStencilPass = Keep;
	BackFaceStencilFail = Keep;
};

float4 GetClipSpacePosition(float2 texcoord, float depth)
{
	float4 clipSpaceLocation;
	clipSpaceLocation.xy = texcoord * 2.0f - 1.0f;
	clipSpaceLocation.y *= -1;
	clipSpaceLocation.z = depth;
	clipSpaceLocation.w = 1.0f;
    
	return clipSpaceLocation;
}

float3 GetPositionVS(float2 texcoord, float depth, matrix inverse_projection)
{
	float4 clipSpaceLocation;
	clipSpaceLocation.xy = texcoord * 2.0f - 1.0f;
	clipSpaceLocation.y *= -1;
	clipSpaceLocation.z = depth;
	clipSpaceLocation.w = 1.0f;
	float4 homogenousLocation = mul(clipSpaceLocation, inverse_projection);
	return homogenousLocation.xyz / homogenousLocation.w;
}

float ConvertZToLinearDepth(float depth, float near, float far)
{    
	return (near * far) / (far - depth * (far - near));
}

float ExponentialFog(float dist)
{
	float fog_dist = max(dist - 100.f, 0.0);
    
	float fog = exp(-fog_dist * 0.002f);
	return 1 - fog;
}

bool IsSaturated(float value)
{
	return value == saturate(value);
}
bool IsSaturated(float2 value)
{
	return IsSaturated(value.x) && IsSaturated(value.y);
}
bool IsSaturated(float3 value)
{
	return IsSaturated(value.x) && IsSaturated(value.y) && IsSaturated(value.z);
}
bool IsSaturated(float4 value)
{
	return IsSaturated(value.x) && IsSaturated(value.y) && IsSaturated(value.z) && IsSaturated(value.w);
}

float4 ComputeScattering(float lightDotview)
{
	float4 result = float4(0, 0, 0, 0);
	
	return result;
}

float2 ComputeScreenUV(float4 _clipPos)
{
	float2 screenUV = _clipPos.xy / _clipPos.w;
	screenUV = 0.5f * screenUV + 0.5f;
	screenUV.y = 1 - screenUV.y;

	return screenUV;
}

float4 CalcLightInternal(float3 ligtcolor, float lightambientIntensity, float3 _camPos, float3 _direction, float3 _worldPos, float3 _normal)
{
	float4 ambientColor = float4(ligtcolor * lightambientIntensity, 1.0);
	float diffuseFactor = dot(_normal, -_direction);

	float4 diffuseColor = 0;

	if (diffuseFactor > 0.0)
	{
		diffuseColor = float4(ligtcolor * lightambientIntensity * diffuseFactor, 1.0);
	}

	return diffuseColor * diffuseFactor + ambientColor;
}

float4 CalcPointLight(float3 pointlightcolor, float pointlightambentintensity,float3 pointlightpos,float pointlightradius, float3 _camPos, float3 _worldPos, float3 _normal)
{
	float3 lightDirection = _worldPos - pointlightpos;
	float distance = length(lightDirection);

	lightDirection = normalize(lightDirection);

	float4 color = CalcLightInternal(pointlightcolor, pointlightambentintensity, _camPos, lightDirection, _worldPos, _normal);
	float attention = pow((pointlightradius - distance) / pointlightradius, 2);
	color = color * attention;
	color.w = 1;

	return color;
}

float G1V(float _dotNV, float _k)
{
	return 1.0f / (_dotNV * (1.0f - _k) + _k);
}


float LightingGGX_Ref(float3 _N, float3 _V, float3 _L, float _F0, float _Roughness)
{
	float alpha = _Roughness * _Roughness;

	float3 H = normalize(_V + _L);

	float dotNL = saturate(dot(_N, _L));
	float dotNV = saturate(dot(_N, _V));
	float dotNH = saturate(dot(_N, H));
	float dotLH = saturate(dot(_L, H));

	float F; // Fresnel
	float D; // GGX ref
	float V; // smith's schlick

	float alphaSqr = alpha * alpha;
	const float PI = 3.141592f;
	float denom = dotNH * dotNH * (alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (PI * denom * denom);

	float dotLH5 = pow(1.0f - dotLH, 5);
	F = _F0 + (1.0 - _F0) * dotLH5;

	float k = alpha / 2.0f;
	V = G1V(dotNL, k) * G1V(dotNV, k);

	float specular = dotNL * D * F * V;

	return specular;
}
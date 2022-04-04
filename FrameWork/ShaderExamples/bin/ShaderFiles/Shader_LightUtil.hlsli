
static const uint SSCS_MAX_STEPS = 16;

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


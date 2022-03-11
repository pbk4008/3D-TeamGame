#include "Shader_Calculate.hpp"

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};


Texture2D<float4> inputTexture : register(t0);
RWTexture2D<float4> outputTexture : register(u0);

groupshared float4 gCache[266];
const static float weights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };

cbuffer ThresholdParams : register(b0)
{
	float threshold;
}

[numthreads(8, 8, 1)]
void CS_Main(/*uint3 groupID : SV_GroupID,*/ uint3 groupThreadID : SV_GroupThreadID/*, uint groupIndex : SV_GroupIndex*/, uint3 dispatchID : SV_DispatchThreadID)
{
	//float width, height;
	//inputTexture.GetDimensions(width, height);
	
	//float2 imagesize = float2(width, height);
	//float2 index = min(dispatchID.xy, imagesize);
	//float2 coord = index / imagesize;
	
	//gCache[groupThreadID.x + 5] = inputTexture.SampleLevel(DefaultSampler, coord, 0);
	
	//if(groupThreadID.x < 5)
	//{
	//	int x = max(dispatchID.x - 5, 0);
	//	coord.x = (x / imagesize.x);
	//	gCache[groupThreadID.x] = inputTexture.SampleLevel(DefaultSampler, coord, 0);
	//}
	//if (groupThreadID.x >= 251)
	//{
	//	int x = min(dispatchID.x + 5, imagesize.x);
	//	coord.x = (x / imagesize.x);
	//	gCache[groupThreadID.x + 10] = inputTexture.SampleLevel(DefaultSampler, coord, 0);
	//}
 
	//GroupMemoryBarrierWithGroupSync();
 
	//float4 blurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
 //   [unroll]
	//for (int i = -5; i <= 5; ++i)
	//{
	//	int k = groupThreadID.x + 5 + i;
	//	blurColor += gCache[k] * weights[i + 5];
	//}
	//outputTexture[dispatchID.xy] = blurColor;
	
	
    // output pixel in half resolution
	// 픽셀을 절반 해상도로 출력
	uint2 pixel = uint2(dispatchID.x, dispatchID.y);

    // bilinear interpolation for downsampling
	// 다운샘플링을 위한 쌍선형 보간
	uint2 inPixel = pixel * 2;
	float4 hIntensity0 = lerp(inputTexture[inPixel], inputTexture[inPixel + uint2(1, 0)], 0.5);
	float4 hIntensity1 = lerp(inputTexture[inPixel + uint2(0, 1)], inputTexture[inPixel + uint2(1, 1)], 0.5);
	float4 intensity = lerp(hIntensity0, hIntensity1, 0.5);

    // thresholding on downsampled value
	// 다운샘플링된 값에 대한 임계값
	float intensityTest = (float) (length(intensity.rgb) > threshold);

	outputTexture[pixel] = float4(intensityTest * intensity.rgb, 1.0);
}
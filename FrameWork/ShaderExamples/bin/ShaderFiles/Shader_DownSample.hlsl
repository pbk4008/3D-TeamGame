#include "Shader_Calculate.hpp"

#define GAUSSIAN_RADIUS 7

Texture2D<float4> inputTexture : register(t0);
RWTexture2D<float4> outputTexture : register(u0);

//groupshared float4 gCache[266];
//const static float weights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };

cbuffer ThresholdParams : register(b0)
{
	float threshold;
}

[numthreads(16, 16, 1)]
void CS_Main(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchID : SV_DispatchThreadID)
{
    // output pixel in half resolution
	// 픽셀을 절반 해상도로 출력
	uint2 pixel = uint2(dispatchID.x, dispatchID.y);

    // bilinear interpolation for downsampling
	// 다운샘플링을 위한 쌍선형 보간
	uint2 inPixel = pixel/* * 2*/;
	float4 hIntensity0 = lerp(inputTexture[inPixel], inputTexture[inPixel + uint2(1, 0)], 0.5);
	float4 hIntensity1 = lerp(inputTexture[inPixel + uint2(0, 1)], inputTexture[inPixel + uint2(1, 1)], 0.5);
	float4 intensity = lerp(hIntensity0, hIntensity1, 0.5);

    // thresholding on downsampled value
	// 다운샘플링된 값에 대한 임계값
	float intensityTest = (float) (length(intensity.rgb) > threshold);

	outputTexture[pixel] = float4(intensityTest * intensity.rgb, 1.0);
}
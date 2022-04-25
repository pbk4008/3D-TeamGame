#define EXPONENTIAL_FOG 0
#define EXPONENTIAL_HEIGHT_FOG 1

// Constant buffer
cbuffer distortion
{
	float g_thick;
};

cbuffer DOF
{
	float4 g_dofparam = (float4)0;
};

// -----------------------Radial-----------------------
half3 Radialblur(Texture2D mainTex,SamplerState sample,half2 UV, half RadialCnt)
{
	const half blurpower = 0.03f;
		
	half2 dir = UV - half2(0.5f, 0.5f);

	half3 color = half3(0, 0, 0);
	half f = 1.0 / (half) RadialCnt;
	for (int i = 0; i < RadialCnt; ++i)
	{
		color += mainTex.Sample(sample, UV - blurpower * dir * half(i)).rgb * f;
	}
	
	return color;
}

// -----------------------OutLine-----------------------
half4 Outline(Texture2D mainTex, SamplerState sample, half2 UV,half4 OutColor)
{
	half fCoord[3] = { -1.f, 0.f, 1.f };
	half fLaplacianMask[9] =
	{
		-1.f, -1.f, -1.f,
		-1.f, 9.f, -1.f,
		-1.f, -1.f, -1.f
	};
	for (int i = 0; i < 9; ++i)
		OutColor += fLaplacianMask[i] * mainTex.Sample(sample, (UV + half2(fCoord[i % 3] / 1280.f, fCoord[i / 3] / 720.f)));	
	
	return OutColor;
}

// -----------------------Distortion-----------------------
half4 Distortion(Texture2D DistortionTex,Texture2D DiffuseTex,SamplerState Sample, half2 UV)
{
	half2 noiseuv = UV + 0.001f;
	half4 noise = DistortionTex.Sample(Sample, noiseuv);
	half2 duv = UV.xy + noise.xy * g_thick;
	
	return DiffuseTex.Sample(Sample, duv);
	
}

// -----------------------FOG-----------------------
half ExponentialFog(float4 pos_ws, float4 campos , float fogstart, float fogdensity)
{
	float3 obj_to_camera = (campos - pos_ws).xyz;

	float distance = length(obj_to_camera);
    
	float fog_dist = max(distance - fogstart, 0.0);
    
	float fog = exp(-fog_dist * fogdensity);
	return 1 - fog;
}

half ExponentialHeightFog(float4 pos_ws, float4 campos, float fogstart, float fogdensity, float fogfalloff)
{
	float3 camera_to_world = pos_ws.xyz;

	float distance = length(camera_to_world);
    
	// 픽셀 거리에 대한 안개 응시 거리 찾기
	float fogDist = max(distance - fogstart, 0.0);

	// 거리 기반 안개 강도
	float fogHeightDensityAtViewer = exp(-fogfalloff * campos.y);
	float fogDistInt = fogDist * fogHeightDensityAtViewer;

	// 높이 기반 안개 강도
	float eyeToPixelY = camera_to_world.y * (fogDist / distance);
	float t = fogfalloff * eyeToPixelY;
	const float thresholdT = 0.01;
	float fogHeightInt = abs(t) > thresholdT ? (1.0 - exp(-t)) / t : 1.0;

	// 두 요소를 결합하여 최종 요소를 얻습니다.
	float fog = exp(-fogdensity * fogDistInt * fogHeightInt);

	return 1 - fog;
}

// -----------------------DOF-----------------------
//half2 rand2d(half2 uv)
//{
//	return frac(sin(half2(
//        dot(uv, half2(215.1616, 82.1225)),
//        dot(uv, half2(12.345, 856.125))
//    )) * 41234.45) * 2.0 - 1.0;
//}

//float getCoC(float depth, float focalPlane)
//{
//	float focalLength = 0.1;
//	float aperture = min(1.0, focalPlane * focalPlane);
//	return abs(aperture * (focalLength * (focalPlane - depth)) /
//        (depth * (focalPlane - focalLength)));
//}

half ConvertZToLinearDepth(half depth)
{
	float near = 0.1f;
	float far = 300.f;
    
	return (near * far) / (far - depth * (far - near));

}

float BlurFactor(in float depth, in float4 DOF)
{
	float f0 = 1.0f - saturate((depth - DOF.x) / max(DOF.y - DOF.x, 0.01f));
	float f1 = saturate((depth - DOF.z) / max(DOF.w - DOF.z, 0.01f));
	float blur = saturate(f0 + f1);

	return blur;
}

half3 DistanceDOF(float3 colorFocus, float3 colorBlurred, float depth)
{
	half blurFactor = BlurFactor(depth, g_dofparam);
	
	return lerp(colorFocus, colorBlurred, blurFactor);
}

half4 DOF(float4 outcolor, Texture2D blurtex, SamplerState st, half2 UV, half depth)
{
	half4 color = outcolor;
	
	// 흐리게 축소된 HDR 텍스처에서 흐릿한 색상을 가져옵니다.
	half3 sceneblurred = blurtex.Sample(st, UV).xyz;
	
	// 전체 해상도 깊이를 선형 깊이로 변환
	depth = ConvertZToLinearDepth(depth);

	// 거리 DOF 색상 계산
	color = half4(DistanceDOF(color.xyz, sceneblurred, depth), color.a);
	
	return color;
}

// ToneMapping
static const float gamma = 2.2;

float ColorToLuminance(float3 color)
{
	return dot(color, float3(0.2126f, 0.7152f, 0.0722f));
}


float3 LinearToneMapping(float3 color)
{
	color = clamp(color, 0., 1.f);
	color = pow(color, 1.f / gamma);
	return color;
}

float3 ReinhardToneMapping(float3 color)
{
	float luma = ColorToLuminance(color);
	float toneMappedLuma = luma / (1.f + luma);
	if (luma > 1e-6)
		color *= toneMappedLuma / luma;
    
	color = pow(color, 1.f / gamma);
	return color;
}

float3 ACESFilmicToneMapping(float3 color)
{
	color *= 0.6f;
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return pow(clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0f, 1.0f), 1.f / gamma);
}

float3 HableToneMapping(float3 color)
{
	float A = 0.15f;
	float B = 0.50f;
	float C = 0.10f;
	float D = 0.20f;
	float E = 0.02f;
	float F = 0.30f;
	float W = 11.2f;
	float exposure = 2.;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	color = pow(color, 1. / gamma);
	return color;
}
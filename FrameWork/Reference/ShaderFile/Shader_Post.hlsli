
cbuffer distortion
{
	float g_thick;
};

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
		OutColor += fLaplacianMask[i] * mainTex.Sample(sample, (UV + half2(fCoord[i / 3] / 1280.f, fCoord[i / 3] / 720.f)));
	
	return OutColor;
}

half4 Distortion(Texture2D DistortionTex,Texture2D DiffuseTex,SamplerState Sample, half2 UV)
{
	half2 noiseuv = UV + 0.001f;
	half4 noise = DistortionTex.Sample(Sample, noiseuv);
	half2 duv = UV.xy + noise.xy * g_thick;
	
	return DiffuseTex.Sample(Sample, duv);
	
}

half2 rand2d(half2 uv)
{
	return frac(sin(half2(
        dot(uv, half2(215.1616, 82.1225)),
        dot(uv, half2(12.345, 856.125))
    )) * 41234.45) * 2.0 - 1.0;
}

float getCoC(float depth, float focalPlane)
{
	float focalLength = 0.1;
	float aperture = min(1.0, focalPlane * focalPlane);
	return abs(aperture * (focalLength * (focalPlane - depth)) /
        (depth * (focalPlane - focalLength)));
}

half4 FOG(half4 Color, Texture2D blurTex, Texture2D depthTex, SamplerState samplest, half2 UV, half4x4 projinv, half4x4 viewinv, half4 campos)
{	

	
	//half4 screen = screenTex.Sample(samplest, UV);
	//half4 depth = depthTex.Sample(samplest, UV);
	//half4 blur = blurTex.Sample(samplest, UV);
	
	//return lerp(screen, blur, saturate(0.5f * abs(0.5f - depth.x)));
}
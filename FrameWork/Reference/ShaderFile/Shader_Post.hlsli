
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

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

half4 Distortion(Texture2D DistortionTex,Texture2D DiffuseTex,SamplerState Sample, half2 UV, half delta)
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

half4 DOF(Texture2D screenTex, Texture2D blurTex, Texture2D depthTex, SamplerState samplest, half2 UV, half4x4 projinv, half4x4 viewinv, half4 campos)
{	
	//half minDist = 8.0f;
	//half maxDist = 12.0f;
	
	//float far = 300.f;
	//half2 texsize = half2(1280.f, 720.f);
	//half4 color = screenTex.Sample(samplest, UV);
	//half4 depth = depthTex.Sample(samplest, UV);
	//half fViewZ = depth.y * 300.f;

	//half4 pos = half4(0, 0, 0, 0);
	//pos.x = (UV.x * 2.f - 1.f) * fViewZ;
	//pos.y = (UV.y * -2.f + 1.f) * fViewZ;
	//pos.z = depth.x * fViewZ;
	//pos.w = fViewZ;
	//pos = mul(pos, projinv);
	////pos = mul(pos, viewinv);
	//half2 camposition = campos.xy / texsize;
	
	//if(pos.a <= 0.0)
	//{		
	//	return half4(1.0, 1.0, 1.0, 1.0);
	//}
	
	//half4 outofFocusColor = blurTex.Sample(samplest, UV);
	//half4 focuspoint = depthTex.Sample(samplest, camposition);
	//half blur = smoothstep(minDist, maxDist, length(pos - focuspoint));
	//half4 test = lerp(color, outofFocusColor, blur);
	//return color + test;
	
	 // Normalized pixel coordinates (from 0 to 1)
	const float PI = 3.141592f;
	
	half2 uv = UV;
    
    // Sample original texture data at uv
	half4 texData = screenTex.Sample(samplest, uv);
	half4 depths = depthTex.Sample(samplest, uv);
    // Get its depth
	half depth = depths.x;
    
    // Focal plane at 3.9 (the camera is looking at the center from ~4.0)
	half focalPlane = 3.9;
    
    // Calculate CoC, see above
	half coc = getCoC(depth, focalPlane);
    
    // Sample count
	const int taps = 32;
    
	half golden = 3.141592 * (3.0 - sqrt(5.0));
    
    // Color & total weight
	half3 color = half3(0,0,0);
	float tot = 0.0;
    
	for (int i = 0; i < taps; i++)
	{
        // Radius slowly increases as i increases, all the way up to coc
		half radius = coc * sqrt(half(i) / half(taps));
        
        // Golden ratio sample offset
		half theta = half(i) * golden;
		half2 tapUV = uv + sin(theta + half2(0.0, PI / 2.0)) * radius;
        
        // Sample the bit over there
		half4 tapped = screenTex.Sample(samplest, tapUV);
		half tappedDepth = depthTex.Sample(samplest, tapUV).x;

		if (tappedDepth > 0.0)
		{
            // Use CoC over there as weight
			half tappedCoC = getCoC(tappedDepth, focalPlane);
			half weight = max(0.001, tappedCoC);
            
            // Contribute to final color
			color += tapped.rgb * weight;
            // And final weight sum
			tot += weight;
		}
	}
    // And normalize the final color by final weight sum
	color /= tot;
	return half4(color, 1.0);
	
	
}
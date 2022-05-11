
half3 Normalmapping(half3 normaltex, half3x3 tbn)
{
	normaltex = normaltex * 2 - 1;
	normaltex = normalize(normaltex);
	
	normaltex = normalize(mul(normaltex, tbn));
	normaltex = normaltex * 0.5f + 0.5f;
	
	return normaltex;
}

//--------------------------------------------------------------------------------------------------------//
// Distortion
float Scale1 = 0.3;
float Scale2 = 3.5;
float Amp = 20.0;
float FreqX = 30.0;
float FreqY = 30.0;


float3x3 m = float3x3(0.00, 0.80, 0.60,
					-0.80, 0.36, -0.48,
					-0.60, -0.48, 0.64);

half hash(half n)
{
	return frac(sin(n) * 43758.5453);
}

half noise(in half3 x)
{
	half3 p = floor(x);
	half3 f = frac(x);

	f = f * f * (3.0 - 2.0 * f);

	half n = p.x + p.y * 57.0 + 113.0 * p.z;

	float res = lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
				lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
                lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
	
	return res;
}

half fbm(half3 p)
{
	float f;
	f = 0.5000 * noise(p);
	p = mul(mul(m, p), 2.02);
	f += 0.2500 * noise(p);
	p = mul(mul(m, p), 2.03);
	f += 0.1250 * noise(p);
	p = mul(mul(m, p), 2.01);
	f += 0.0625 * noise(p);
	return f;
}
half mynoise(half3 p)
{
	return noise(p);
}
half myfbm(half3 p)
{
	half f;
	f = 0.5000 * mynoise(p);
	p = mul(mul(m, p), 2.02);
	f += 0.2500 * mynoise(p);
	p = mul(mul(m, p), 2.03);
	f += 0.1250 * mynoise(p);
	p = mul(mul(m, p), 2.01);
	f += 0.0625 * mynoise(p);
	p = mul(mul(m, p), 2.05);
	f += 0.0625 / 2.0 * mynoise(p);
	p = mul(mul(m, p), 2.02);
	f += 0.0625 / 4.0 * mynoise(p);
	
	return f;
}
float myfbm2(half3 p)
{
	float f;
	f = 1.0 - 0.5000 * mynoise(p);
	p = mul(mul(m, p), 2.02);
	f *= 1.0 - 0.2500 * mynoise(p);
	p = mul(mul(m, p), 2.03);
	f *= 1.0 - 0.1250 * mynoise(p);
	p = mul(mul(m, p), 2.01);
	f *= 1.0 - 0.0625 * mynoise(p);
	p = mul(mul(m, p), 2.05);
	f *= 1.0 - 0.0625 / 2.0 * mynoise(p);
	p = mul(mul(m, p), 2.02);
	f *= 1.0 - 0.0625 / 4.0 * mynoise(p);
	return f;
}

half4 Noisfunction(Texture2D MainTex, SamplerState sample, half2 UV, half delta, half4 color3)
{
	half2 resolution = half2(1920.f, 1080.f);
	half2 uv = UV / resolution.y;
	half3 v;
	half3 p = Scale2 * half3(UV, 0) - delta * half3(1.0, 1.0, 1.0) * 0.1;
	float x = myfbm(p);
	v = (0.5 + 0.5 * sin(x * half3(FreqX, FreqY, 1.0) * Scale1)) / Scale1;
	v *= Amp;
	half3 Ti = MainTex.Sample(sample, 0.02 * v.xy + UV.xy / resolution.xy).rgb;
	Ti = Ti * color3.rgb;
	half4 color = half4(Ti, 1.0);
	
	return color;
}

// Circle distortion
half2 GetOffsetFromCenter(half2 screenCoords, half2 screenSize)
{
	half2 halfScreenSize = screenSize / 2.0;
    
	return (screenCoords.xy - halfScreenSize) / min(halfScreenSize.x, halfScreenSize.y);
}


float EffectDuration = 1.0;
float EffectFadeInTimeFactor = 0.5;
float EffectWidth = 0.4;
float EffectMaxTexelOffset = 20.0;

half2 GetDistortionTexelOffset(half2 offsetDirection, float offsetDistance, float time)
{
	half progress = fmod(time, EffectDuration) / EffectDuration;
    
	half halfWidth = EffectWidth / 2.0;
	half lower = 1.0 - smoothstep(progress - halfWidth, progress, offsetDistance);
	half upper = smoothstep(progress, progress + halfWidth, offsetDistance);
    
	half band = 1.0 - (upper + lower);
    
    
	half strength = 1.0 - progress;
	half fadeStrength = smoothstep(0.0, EffectFadeInTimeFactor, progress);
    
	half distortion = band * strength * fadeStrength;
    
    
	return distortion * offsetDirection * EffectMaxTexelOffset;
}


half3 GetTextureOffset(half2 coords, half2 textureSize, half2 texelOffset)
{
	half2 texelSize = 1.0 / textureSize;
	half2 offsetCoords = coords + texelSize * texelOffset;
    
	half2 halfTexelSize = texelSize / 2.0;
	half2 clampedOffsetCoords = clamp(offsetCoords, halfTexelSize, 1.0 - halfTexelSize);
    
	return half3(0, 0, 0); /*texture(iChannel0, clampedOffsetCoords).rgb;*/
}

//--------------------------------------------------------------------------------------------------------//

half4 Dissolve(Texture2D Diffuse, Texture2D dissolvetex, SamplerState sample, half2 UV, half time)
{
	half4 diffuse = Diffuse.Sample(sample, UV);
	half dissolve = dissolvetex.Sample(sample, UV).r;
	half clipAmount = dissolve - time;

	clip(diffuse.a - clipAmount);
	
	if (clipAmount <= 0)
		discard;
	
	if (dissolve <= time)
	{
		if (clipAmount >= 0.03f)
		{
			diffuse = half4(1, 0, 0, 1);
		}
		else if (clipAmount >= 0.06f)
		{
			diffuse = half4(1, 1, 0, 1);
		}
		else if (clipAmount > 0 && clipAmount <= 0.1f)
		{
			diffuse = half4(1, 1, 1, 1);
		}
	}

	return diffuse;
}

half4 RimLighting(float4 normal, float4 camdir, float rimintensity, float3 rimcolor, float time)
{
	half rim = 0.f;
	rim = 1 - saturate(dot(normal, -camdir));
	rim = pow(rim, rimintensity);
	rimcolor = rim * rimcolor * time;
	
	return half4(rimcolor, 1.f);
}

half4 MotionTrailRim(float4 normal, float4 camdir, float rimintensity, float3 rimcolor)
{
	half4 rim = 1.f - saturate(dot(normalize(normal), normalize(camdir)));
	rim.rgb = rim.rgb * rimcolor;
	return rim;
}
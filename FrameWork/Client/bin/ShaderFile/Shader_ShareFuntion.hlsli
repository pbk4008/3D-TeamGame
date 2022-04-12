half3 Normalmapping(half3 normaltex, half3x3 tbn)
{
	normaltex = normaltex * 2 - 1;
	normaltex = normalize(normaltex);
	
	normaltex = normalize(mul(normaltex, tbn));
	normaltex = normaltex * 0.5f + 0.5f;
	
	return normaltex;
}

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
	half2 resolution = half2(1280.f, 720.f);
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

void Dissolve(Texture2D dissolvetex,SamplerState sample,half2 UV,half time)
{
	half dissolve = dissolvetex.Sample(sample, UV).r;
		
	if (dissolve - time <= 0)
		discard;
}

half4 RimLighting(float4 normal, float4 camdir, float rimintensity, float4 rimcolor)
{
	float rim = 0.f;
	rim = 1 - saturate(dot(normal, -camdir));
	rim = pow(rim, rimintensity);
	rimcolor = rim * rimcolor;
	return float4(rimcolor.rgb, 1.f);
}
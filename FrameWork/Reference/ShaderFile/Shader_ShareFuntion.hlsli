half3 Normalmapping(half3 normaltex, half3x3 tbn)
{
	normaltex = normaltex * 2 - 1;
	normaltex = normalize(normaltex);
	
	normaltex = normalize(mul(normaltex, tbn));
	normaltex = normaltex * 0.5f + 0.5f;
	
	return normaltex;
}
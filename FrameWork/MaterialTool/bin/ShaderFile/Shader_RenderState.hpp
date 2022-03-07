RasterizerState CullMode_Default
{
	FillMode = solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};
RasterizerState CullMode_None
{
	CullMode = NONE;
};
DepthStencilState ZDefault
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};
DepthStencilState ZWriteDisable
{
	DepthEnable = true;
	DepthWriteMask = Zero;
	DepthFunc = less;
};

BlendState BlendDisable
{
	BlendEnable[0] = false;
};
BlendState AlphaBlend
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};
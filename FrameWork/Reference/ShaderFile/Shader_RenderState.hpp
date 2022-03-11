

/*
//D3D11_FILL_MODE FillMode;
//D3D11_CULL_MODE CullMode;
//BOOL FrontCounterClockwise;
//INT DepthBias;
//FLOAT DepthBiasClamp;
//FLOAT SlopeScaledDepthBias;
//BOOL DepthClipEnable;
//BOOL ScissorEnable;
//BOOL MultisampleEnable;
//BOOL AntialiasedLineEnable;
*/
RasterizerState CullMode_BackOn
{
	FillMode = solid;
	CullMode = NONE;
	FrontCounterClockwise = false;
};
RasterizerState CullMode_BackOnWireFrame
{
	FillMode = wireframe;
	CullMode = NONE;
	FrontCounterClockwise = false;
};
RasterizerState CullMode_Default
{
	FillMode = solid;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_Wireframe
{
	FillMode = wireframe;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_None
{
	CullMode = None;
};


//BOOL DepthEnable;
//D3D11_DEPTH_WRITE_MASK DepthWriteMask;
//D3D11_COMPARISON_FUNC DepthFunc;
//BOOL StencilEnable;
//UINT8 StencilReadMask;
//UINT8 StencilWriteMask;
//D3D11_DEPTH_STENCILOP_DESC FrontFace;
//D3D11_DEPTH_STENCILOP_DESC BackFace;

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


//BOOL BlendEnable;
//D3D11_BLEND SrcBlend;
//D3D11_BLEND DestBlend;
//D3D11_BLEND_OP BlendOp;
//D3D11_BLEND SrcBlendAlpha;
//D3D11_BLEND DestBlendAlpha;
//D3D11_BLEND_OP BlendOpAlpha;
//UINT8 RenderTargetWriteMask;


BlendState AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BlendDisable
{
	BlendEnable[0] = false;	
};
#include "CSTarget.h"
#include "VIBuffer_RectViewPort.h"

CCSTarget::CCSTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCSTarget::Compile_CSShaderFile(const _tchar* pShaderFilePath, const char* pEntryPoitn)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob* pCompileErrorMessage = nullptr;

	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoitn, "cs_5_0", iFlag, 0, &m_pCS.csBlob, &pCompileErrorMessage)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateComputeShader(m_pCS.csBlob->GetBufferPointer(), m_pCS.csBlob->GetBufferSize(), nullptr, &m_pCS.csShader)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCSTarget::Create_CSBuffer(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType eType)
{
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_CSGroup.CSrenderTargetTexutre)))
		return E_FAIL;

	/// RTV
	D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
	ZeroMemory(&rtvdesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	rtvdesc.Format = eFormat;
	rtvdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvdesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_CSGroup.CSrenderTargetTexutre, &rtvdesc, &m_CSGroup.CSRTV)))
		return E_FAIL;

	//SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC		srvdesc;
	ZeroMemory(&srvdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvdesc.Format = eFormat;
	srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Texture2D.MostDetailedMip = 0;
	srvdesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_CSGroup.CSrenderTargetTexutre, &srvdesc, &m_CSGroup.CSSRV)))
		return E_FAIL;

	//UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavdesc;
	ZeroMemory(&uavdesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	uavdesc.Format = eFormat;
	uavdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavdesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_CSGroup.CSrenderTargetTexutre, &uavdesc, &m_CSGroup.CSUAV)))
		return E_FAIL;

	// compute shader constant buffer
	D3D11_BUFFER_DESC ccb;
	ZeroMemory(&ccb, sizeof(D3D11_BUFFER_DESC));

	switch (eType)
	{
	case Engine::CCSTarget::DOWNSAMPLE:
		ccb.ByteWidth = sizeof(THRESHOLD);
		break;
	case Engine::CCSTarget::GSBLUR:
		m_BlurParam.radius = GAUSSIAN_RADIUS;
		m_BlurParam.direction = 0;

		float sigma = 10.f;
		float sigmaRcp = 1.f / sigma;
		float twoSigmaSq = 2 * sigma * sigma;

		float sum = 0.f;
		for (size_t i = 0; i <= GAUSSIAN_RADIUS; ++i)
		{
			m_BlurParam.coefficients[i] = (1.f / sigma) * std::expf(-static_cast<float>(i * i) / twoSigmaSq);
			sum += 2 * m_BlurParam.coefficients[i];
		}
		sum -= m_BlurParam.coefficients[0];

		float normalizationFactor = 1.f / sum;
		for (size_t i = 0; i <= GAUSSIAN_RADIUS; ++i)
		{
			m_BlurParam.coefficients[i] *= normalizationFactor;
		}
		ccb.ByteWidth = sizeof(BLURPARAM);
		break;
	}
	ccb.Usage = D3D11_USAGE_DYNAMIC;
	ccb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ccb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	switch (eType)
	{
	case Engine::CCSTarget::DOWNSAMPLE:
		if (FAILED(m_pDevice->CreateBuffer(&ccb, nullptr, &m_pConstantBuffer)))
			return E_FAIL;
		break;
	case Engine::CCSTarget::GSBLUR:
		if (FAILED(m_pDevice->CreateBuffer(&ccb, nullptr, &m_pBlurBuffer)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CCSTarget::RunCSDownSample(ID3D11ShaderResourceView* ppShaderResourceView)
{
	constexpr ID3D11RenderTargetView* NULL_RT = nullptr;
	constexpr ID3D11ShaderResourceView* NULL_SRV = nullptr;
	constexpr ID3D11UnorderedAccessView* NULL_UAV = nullptr;
	constexpr UINT NO_OFFSET = -1;

	THRESHOLD thresholdparams = { 0.5f };
	D3D11_MAPPED_SUBRESOURCE ms;
	if (FAILED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
		return E_FAIL;

	memcpy(ms.pData, &thresholdparams, sizeof(THRESHOLD));
	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);

	m_pDeviceContext->CSSetShader(m_pCS.csShader, 0, 0);
	m_pDeviceContext->CSSetShaderResources(0, 1, &ppShaderResourceView);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_CSGroup.CSUAV, &NO_OFFSET);
	m_pDeviceContext->CSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pDeviceContext->Dispatch((_uint)(ceil(1280 / 16.0)), (_uint)(ceil(720 / 16.0)), 1);

	m_pDeviceContext->CSSetShaderResources(0, 1, &NULL_SRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &NULL_UAV, &NO_OFFSET);

	return S_OK;
}

HRESULT CCSTarget::RunCSBlur(ID3D11ShaderResourceView* pDownSample, ID3D11UnorderedAccessView* pDownUAV)
{
	constexpr ID3D11RenderTargetView* NULL_RT = nullptr;
	constexpr ID3D11ShaderResourceView* NULL_SRV = nullptr;
	constexpr ID3D11UnorderedAccessView* NULL_UAV = nullptr;
	constexpr UINT NO_OFFSET = -1;

	m_pDeviceContext->CSSetShader(m_pCS.csShader, 0, 0);
	std::array<ID3D11ShaderResourceView*, 2> csSRVs = { pDownSample, m_CSGroup.CSSRV };
	std::array<ID3D11UnorderedAccessView*, 2> csUAVs = { m_CSGroup.CSUAV, pDownUAV };
	for (UINT direction = 0; direction < 2; ++direction)
	{
		m_BlurParam.direction = direction;

		D3D11_MAPPED_SUBRESOURCE ms;
		size_t test = sizeof(BlurParams);
		if (FAILED(m_pDeviceContext->Map(m_pBlurBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
			return E_FAIL;
		memcpy(ms.pData, &m_BlurParam, sizeof(BlurParams));
		m_pDeviceContext->Unmap(m_pBlurBuffer, 0);

		m_pDeviceContext->CSSetShaderResources(0, 1, &csSRVs[direction]);
		m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &csUAVs[direction], &NO_OFFSET);
		m_pDeviceContext->CSSetConstantBuffers(0, 1, &m_pBlurBuffer);

		m_pDeviceContext->Dispatch((_uint)(ceil(1280 / 16.0)), (_uint)(ceil(720 / 16.0)), 1);

		m_pDeviceContext->CSSetShaderResources(0, 1, &NULL_SRV);
		m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &NULL_UAV, &NO_OFFSET);
	}

	return S_OK;
}

HRESULT CCSTarget::NativeConstruct(const _tchar* pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType etype)
{
	if (FAILED(Compile_CSShaderFile(pShaderFilePath, pEntryPoitn)))
		return E_FAIL;

	if (FAILED(Create_CSBuffer(iWidth, iHeight, eFormat, vClearColor, etype)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCSTarget::Clear()
{
	if (nullptr == m_CSGroup.CSRTV ||
		nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearRenderTargetView(m_CSGroup.CSRTV, (_float*)&m_vClearColor);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CCSTarget::Ready_Debug_Buffer(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	m_pDebugBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, fX, fY, fSizeX, fSizeY, L"../../Reference/ShaderFile/Shader_DSViewPort.hlsl");

	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	Clear();

	return S_OK;
}

HRESULT CCSTarget::Render_Debug_Buffer()
{
	if (FAILED(m_pDebugBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_CSGroup.CSSRV)))
		return E_FAIL;

	m_pDebugBuffer->Render(0);

	return S_OK;
}
#endif // DEBUG

CCSTarget* CCSTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CSType etype)
{
	CCSTarget* pInstance = new CCSTarget(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pShaderFilePath, pEntryPoitn, iWidth, iHeight, eFormat, vClearColor, etype)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed To Createing CCSTarget");
	}

	return pInstance;
}

void CCSTarget::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pDebugBuffer);
#endif // _DEBUG;

	Safe_Release(m_pConstantBuffer);
	Safe_Release(m_pBlurBuffer);

	Safe_Release(m_CSGroup.CSrenderTargetTexutre);
	Safe_Release(m_CSGroup.CSRTV);
	Safe_Release(m_CSGroup.CSSRV);
	Safe_Release(m_CSGroup.CSUAV);

	Safe_Release(m_pCS.csBlob);
	Safe_Release(m_pCS.csShader);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

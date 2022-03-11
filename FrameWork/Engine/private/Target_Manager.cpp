#include "Target_Manager.h"
#include "GameInstance.h"


CTarget_Manager::CTarget_Manager()
{

}

ID3D11ShaderResourceView* CTarget_Manager::Get_SRV(const wstring& pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_STV();
}

ID3D11ShaderResourceView* CTarget_Manager::Get_SRVCS(const _tchar* pTargetTag)
{
	CCSTarget* pCSTarget = Find_CSTarget(pTargetTag);
	if (nullptr == pCSTarget)
		return nullptr;

	return pCSTarget->Get_SRV();
}


HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CRenderTarget::RTT eType)
{
	if (nullptr != Find_Target(pTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(pDevice, pDeviceContext, iWidth, iHeight, eFormat, vClearColor, eType);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_Targets.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_CSTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* CsTag, const _tchar* pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CCSTarget::CSType eType)
{
	CCSTarget* pCSTarget = CCSTarget::Create(pDevice, pDeviceContext, pShaderFilePath, pEntryPoitn, iWidth, iHeight, eFormat, vClearColor, eType);

	if (!pCSTarget)
		return E_FAIL;

	m_CSTarges.insert(make_pair(CsTag, pCSTarget));

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const wstring& pMRTTag, const wstring& pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pDeviceContext, const wstring& pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	pDeviceContext->OMGetRenderTargets(1, &m_pOldView, &m_pDepthStencilView);

	_uint		iNumViews = (_uint)pMRTList->size();

	_uint		iIndex = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iIndex++] = pRenderTarget->Get_RTV();
	}

	pDeviceContext->OMSetRenderTargets(iNumViews, RTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->OMSetRenderTargets(1, &m_pOldView, m_pDepthStencilView);
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Safe_Release(m_pOldView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_CSTarget(ID3D11DeviceContext* pDeviceContext, const _tchar* pCSTag)
{
	CCSTarget* pCsTarget = Find_CSTarget(pCSTag);

	ID3D11RenderTargetView* RTVs = nullptr;

	pDeviceContext->OMGetRenderTargets(1, &m_pOldView, &m_pDepthStencilView);

	pCsTarget->Clear();
	RTVs = pCsTarget->Get_RTV();

	pDeviceContext->OMSetRenderTargets(1, &RTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_CSTarget(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->OMSetRenderTargets(1, &m_pOldView, m_pDepthStencilView);
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Safe_Release(m_pOldView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::RunComputeShader(const _tchar* CsTag, ID3D11ShaderResourceView* pSRV, CCSTarget::CSType eType)
{
	CCSTarget* pCsTarget = Find_CSTarget(CsTag);

	if (!pCsTarget)
		return E_FAIL;

	switch (eType)
	{
	case Engine::CCSTarget::DOWNSAMPLE:
		if (FAILED(pCsTarget->RunCSDownSample(pSRV)))
			return E_FAIL;
		break;
	case Engine::CCSTarget::GSBLUR:
		CCSTarget* pCsPriveTarget = Find_CSTarget(L"CSDownScale");
		if (FAILED(pCsTarget->RunCSBlur(pSRV, pCsPriveTarget->Get_UAV())))
			return E_FAIL;
		break;
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_Debug_Buffer(const wstring& pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug_Buffer(fX, fY, fSizeX, fSizeY);
}
HRESULT CTarget_Manager::Render_Debug_Buffer(const wstring& pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Render_Debug_Buffer();

	return S_OK;
}
HRESULT CTarget_Manager::Ready_Debug_Buffer_CSTarget(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CCSTarget* pCSTarget = Find_CSTarget(pTargetTag);

	if (nullptr == pCSTarget)
		return E_FAIL;

	return pCSTarget->Ready_Debug_Buffer(fX, fY, fSizeX, fSizeY);
}
HRESULT CTarget_Manager::Render_Debug_Buffer_CSTarget(const _tchar* pCsTarget)
{
	CCSTarget* pCS = Find_CSTarget(pCsTarget);

	if (!pCS)
		return E_FAIL;

	pCS->Render_Debug_Buffer();

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CTarget_Manager::Find_Target(const wstring& pTargetTag)
{
	auto	iter = find_if(m_Targets.begin(), m_Targets.end(), CTag_Finder(pTargetTag));
	if (iter == m_Targets.end())
		return nullptr;

	return iter->second;
}

CCSTarget* CTarget_Manager::Find_CSTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_CSTarges.begin(), m_CSTarges.end(), CTag_Finder(pTargetTag));
	if (iter == m_CSTarges.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const wstring& pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));
	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_Targets)
		Safe_Release(Pair.second);

	for (auto& Pair : m_CSTarges)
		Safe_Release(Pair.second);

	m_Targets.clear();
}


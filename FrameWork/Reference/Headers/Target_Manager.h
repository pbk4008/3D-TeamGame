#pragma once

#include "SingleTon.h"
#include "RenderTarget.h"

BEGIN(Engine)

class CTarget_Manager final : public CSingleTon<CTarget_Manager>
{
	friend CSingleTon;
private:
	NO_COPY(CTarget_Manager);
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	ID3D11ShaderResourceView* Get_SRV(const wstring& pTargetTag);

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CRenderTarget::RTT eType);
	HRESULT Add_MRT(const wstring& pMRTTag, const wstring& pTargetTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pDeviceContext, const wstring& pMRTTag); /* 지정된 렌더타겟들을 장치에 순서대로 바인딩한다. */
	HRESULT End_MRT(ID3D11DeviceContext* pDeviceContext); /* 바인딩되기 이전으로 돌린다. */


#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const wstring& pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const wstring& pMRTTag);
#endif // _DEBUG

private:
	map<wstring, class CRenderTarget*>			m_Targets;
	typedef map<wstring, class CRenderTarget*>	TARGETS;

private:
	/* MRT : MultiRenderTarget*/
	map<wstring, list<class CRenderTarget*>>			m_MRTs;
	typedef map<wstring, list<class CRenderTarget*>>	MRTS;

private:
	ID3D11RenderTargetView*				m_pOldView = nullptr;
	ID3D11DepthStencilView*				m_pDepthStencilView = nullptr;


private:
	class CRenderTarget* Find_Target(const wstring& pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& pMRTTag);

public:
	virtual void Free() override;
};

END
#pragma once

#include "SingleTon.h"
#include "RenderTarget.h"
#include "CSTarget.h"

BEGIN(Engine)

class CTarget_Manager final : public CSingleTon<CTarget_Manager>
{
	friend CSingleTon;

private: NO_COPY(CTarget_Manager);
private: CTarget_Manager();
private: virtual ~CTarget_Manager() = default;

public: ID3D11ShaderResourceView* Get_SRV(const wstring& pTargetTag);
public: ID3D11ShaderResourceView* Get_SRVCS(const _tchar* pTargetTag);

public: HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CRenderTarget::RTT eType);
public: HRESULT	Add_CSTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* CsTag, const _tchar* pShaderFilePath, const char* pEntryPoitn, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, CCSTarget::CSType eType);
public: HRESULT Add_MRT(const wstring& pMRTTag, const wstring& pTargetTag);

public: HRESULT Begin_MRT(ID3D11DeviceContext* pDeviceContext, const wstring& pMRTTag);
public: HRESULT End_MRT(ID3D11DeviceContext* pDeviceContext);

public:	HRESULT Begin_CSTarget(ID3D11DeviceContext* pDeviceContext, const _tchar* pCSTag);
public: HRESULT End_CSTarget(ID3D11DeviceContext* pDeviceContext);

public: HRESULT	RunComputeShader(const _tchar* CsTag, ID3D11ShaderResourceView* pSRV, CCSTarget::CSType eType);

#ifdef _DEBUG
public: HRESULT Ready_Debug_Buffer(const wstring& pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
public: HRESULT Render_Debug_Buffer(const wstring& pMRTTag);

public: HRESULT Ready_Debug_Buffer_CSTarget(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
public: HRESULT Render_Debug_Buffer_CSTarget(const _tchar* pCsTarget);
#endif // _DEBUG

private: map<wstring, class CRenderTarget*>			m_Targets; // RT
private: typedef map<wstring, class CRenderTarget*>	TARGETS;

private: map<const _tchar*, class CCSTarget*>		m_CSTarges; // CSTarget

private: map<wstring, list<class CRenderTarget*>>			m_MRTs;
private: typedef map<wstring, list<class CRenderTarget*>>	MRTS;

private: ID3D11RenderTargetView*				m_pOldView = nullptr;
private: ID3D11DepthStencilView*				m_pDepthStencilView = nullptr;

private: CRenderTarget*					Find_Target(const wstring& pTargetTag);
private: CCSTarget*						Find_CSTarget(const _tchar* pTargetTag);
private: list<class CRenderTarget*>*	Find_MRT(const wstring& pMRTTag);

public: virtual void Free() override;
};

END
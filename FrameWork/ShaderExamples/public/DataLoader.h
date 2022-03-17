#pragma once

#include "Base.h"
#include "Defines.h"

BEGIN(Example)

class CStaticObject;

class CDataLoader final : public CBase
{
private: explicit CDataLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private: virtual ~CDataLoader() = default;

public: HRESULT LoadMapData(const wstring& filepath);

private: std::vector<wstring>	m_vecProto;
private: std::vector<MESHDESC>	m_vecMesh;

private: HRESULT	CreateProtoModel(const FILEINFO& fileinfo);
private: HRESULT	CreateCloneModel(const MESHDESC& modeldesc);

private: ID3D11Device*			m_pDevice = nullptr;
private: ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	  
public: static CDataLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual void Free() override;
};
END

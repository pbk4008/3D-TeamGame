#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CMaterial;

class CMaterial_Manager final : public CSingleTon<CMaterial_Manager>
{
private:
	NO_COPY(CMaterial_Manager);
	explicit CMaterial_Manager();
	virtual ~CMaterial_Manager() = default;

public:
	HRESULT NativeConstruct(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	HRESULT Add_Material(const wstring& _wstrMtrlTag, CMaterial* _pMtrl);
	CMaterial* Get_Material(const wstring& _wstrMtrlTag = L"Default");

private:
	unordered_map<wstring, CMaterial*> m_umapMaterials;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
public:
	virtual void Free() override;
};

END
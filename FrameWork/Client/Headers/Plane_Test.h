#pragma once

#include "GameObject.h"

BEGIN(Client)

class CPlane_Test final : public CGameObject
{
private:
	explicit CPlane_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CPlane_Test(const CPlane_Test& _rhs);
	virtual ~CPlane_Test() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	
public:
	virtual HRESULT Ready_Components();
	
private:
	CVIBuffer_Plane* m_pVIBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

public:
	static CPlane_Test* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
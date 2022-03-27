#pragma once

#include "GameObject.h"

BEGIN(Client)

class CCamera_Silvermane;

class CCamera_Culling final : public CGameObject
{
private:
	explicit CCamera_Culling(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCamera_Culling(const CCamera_Culling& _rhs);
	virtual ~CCamera_Culling() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

private:
	CCamera* m_pCamera = nullptr;
	CCamera_Silvermane* m_pCamera_Silvermane = nullptr;

	CTransform* m_pLocalTransform = nullptr;

public:
	static CCamera_Culling* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg) override;
	virtual void Free() override;
};

END
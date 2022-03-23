#pragma once

#include "GameObject.h"

BEGIN(Client)

class CJumpTrigger : public CGameObject
{
private:
	explicit CJumpTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CJumpTrigger(const CJumpTrigger& _rhs);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

public:
	void Raycast_FromMouse(const _double& _dDeltaTime);
	_bool Raycast(const _fvector& _svRayPos, const _fvector& _svRayDir, _float& _fOutDist, const _double& _dDeltaTime);

private:
	CRay_Collider* m_pCollider = nullptr;

	_bool m_isPick = false;
	_float m_fHoldTime = 0.f;

public:
	static CJumpTrigger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
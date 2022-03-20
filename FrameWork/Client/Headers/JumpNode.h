#pragma once

#include "GameObject.h"

BEGIN(Client)

class CJumpNode : public CGameObject
{
private:
	explicit CJumpNode(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CJumpNode(const CJumpNode& _rhs);

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
	CModel* m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CRay_Collider* m_pCollider = nullptr;

	_bool m_isPick = false;
	_float m_fHoldTime = 0.f;

public:
	static CJumpNode* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
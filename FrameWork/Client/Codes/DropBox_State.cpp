#include "pch.h"
#include "DropBox_State.h"

CDropBox_State::CDropBox_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CState(_pDevice, _pDeviceContext)
{
}

HRESULT CDropBox_State::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CDropBox_State::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CDropBox_State::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CDropBox_State::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox_State::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox_State::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CDropBox_State::Set_Object(CGameObject* _pObj)
{
	m_pObj = _pObj;
}

void CDropBox_State::Set_Transform(CTransform* _pTransform)
{
	m_pTransform = _pTransform;
}

void CDropBox_State::Set_Model(CModel* _pModel)
{
	m_pModel = _pModel;
}

void CDropBox_State::Set_AnimationController(CAnimationController* _pAnimationController)
{
	m_pAnimationController = _pAnimationController;
}

void CDropBox_State::Set_Animator(CAnimator* _pAnimator)
{
	m_pAnimator = _pAnimator;
}

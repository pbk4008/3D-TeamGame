#include "pch.h"
#include "Traverse_JumpNodeJog.h"
#include "JumpNode.h"

CTraverse_JumpNodeJog::CTraverse_JumpNodeJog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CTraverse_JumpNodeJog::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CTraverse_JumpNodeJog::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);
	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	if (7 < iCurKeyFrameIndex)
	{
		_vector svVelocity = XMLoadFloat3(&m_vDir) * (_float)_dDeltaTime * m_fMoveSpeed;
		m_pTransform->Add_Velocity(svVelocity);
	}

	if (!m_isJumpEnd)
	{
		if (7 < iCurKeyFrameIndex)
		{
			m_pSilvermane->Set_IsTrasceCamera(false);
			//배진성 바보 -> 범인 0mo
			if (20 < iCurKeyFrameIndex)
				m_pAnimationController->Set_PlaySpeed(0.2f);
			else
				m_pAnimationController->Set_PlaySpeed(6.f);
		}
	}
	else
	{
		m_pSilvermane->Set_IsFall(true);
	}

	if (m_iCutIndex < iCurKeyFrameIndex)
	{
		if (m_pSilvermane->IsEquipWeapon())
		{
			switch (m_pSilvermane->Get_WeaponType())
			{
			case CWeapon::EType::Sword_1H:
				if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
					return E_FAIL;
				break;
			case CWeapon::EType::Hammer_2H:
				if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
					return E_FAIL;
				break;
			}
			return STATE_CHANGE;
		}
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return E_FAIL;
		return STATE_CHANGE;
	}

	return _int();
}

_int CTraverse_JumpNodeJog::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CTraverse_JumpNodeJog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTraverse_JumpNodeJog::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Traverse_Jump_Node_Jog_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XY);

	CTransform* pTargetTransform = m_pSilvermane->Get_TargetJumpNode()->Get_Transform();
	_vector svTargetPosition = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_vTargetPos, svTargetPosition);
	XMStoreFloat3(&m_vDir, XMVector3Normalize(svTargetPosition - m_pTransform->Get_State(CTransform::STATE_POSITION)));

	m_fMoveSpeed = 34.f;
	m_pAnimationController->Set_PlaySpeed(0.8f);
	m_pSilvermane->Set_IsFall(false);
	m_iCutIndex = 60;
	return S_OK;
}

HRESULT CTraverse_JumpNodeJog::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	m_isJumpEnd = false;
	return S_OK;
}

_int CTraverse_JumpNodeJog::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

void CTraverse_JumpNodeJog::OnTriggerExit(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();

	if ((_uint)GAMEOBJECT::JUMP_NODE == iTag)
	{
		m_fMoveSpeed = 0.f;
		m_pAnimationController->Set_PlaySpeed(1.2f);
		m_isJumpEnd = true;
	}
}

CTraverse_JumpNodeJog* CTraverse_JumpNodeJog::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CTraverse_JumpNodeJog* pInstance = new CTraverse_JumpNodeJog(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CTraverse_JumpNodeJog Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTraverse_JumpNodeJog::Free()
{

	__super::Free();
}

#include "pch.h"
#include "Traverse_Jump400Jog.h"

#include "JumpTrigger.h"
#include "JumpBox.h"

CTraverse_Jump400Jog::CTraverse_Jump400Jog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CTraverse_Jump400Jog::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CTraverse_Jump400Jog::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Add_PlusAngle(EDir::Forward, _dDeltaTime);
	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();

	if (!m_isJumpEnd)
	{
		if (7 < iCurKeyFrameIndex)
		{
			m_pSilvermane->Set_IsTrasceCamera(false);
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

_int CTraverse_Jump400Jog::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CTraverse_Jump400Jog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTraverse_Jump400Jog::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Traverse_Jump_400_Jog_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XY);

	//CTransform* pTargetTransform = m_pSilvermane->Get_TargetJumpTrigger()->Get_Transform();
	CTransform* pTargetTransform = m_pSilvermane->Get_TargetJumpBox()->Get_Transform();
	_vector svTargetPosition = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_vTargetPos, svTargetPosition);
	_vector svDir = svTargetPosition - m_pTransform->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_vDir, XMVector3Normalize(XMVectorSetY(svDir, 0.f)));

	m_pAnimationController->Set_MoveSpeed(32.f);
	m_pSilvermane->Set_IsFall(false);
	m_iCutIndex = 45;
	return S_OK;
}

HRESULT CTraverse_Jump400Jog::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pAnimationController->Set_MoveSpeed(40.f);
	m_pSilvermane->Set_IsTrasceCamera(true);
	m_isJumpEnd = false;
	m_pSilvermane->Set_IsFall(true);
	return S_OK;
}

_int CTraverse_Jump400Jog::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

void CTraverse_Jump400Jog::OnTriggerExit(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();
	if ((_uint)GAMEOBJECT::JUMP_TRIGGER == iTag)
	{
		m_isJumpEnd = true;
	}
}

CTraverse_Jump400Jog* CTraverse_Jump400Jog::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CTraverse_Jump400Jog* pInstance = new CTraverse_Jump400Jog(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CTraverse_Jump400Jog Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTraverse_Jump400Jog::Free()
{

	__super::Free();
}

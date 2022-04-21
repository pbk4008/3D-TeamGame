#include "pch.h"
#include "Silvermane_Execution.h"

#include "HierarchyNode.h"

CSilvermane_Execution::CSilvermane_Execution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Execution::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Execution::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	CActor* pTarget = m_pSilvermane->Get_TargetExecution();
	if (pTarget)
	{
		CHierarchyNode* pTargetBone = m_pSilvermane->Get_ExecutionTargetBone();
		if (pTargetBone)
		{
			_matrix smatBone = pTargetBone->Get_CombinedMatrix();
			_matrix smatPivot = m_pModel->Get_PivotMatrix();
			_matrix smatResult = smatBone * smatPivot * m_pTransform->Get_WorldMatrix();

			CTransform* pTargetTransform = pTarget->Get_Transform();

			_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK) * -1.f);
			_vector svRight = XMVector3Normalize(XMVector3Cross(_vector{ 0.f, 1.f, 0.f, 0.f }, svLook));
			_vector svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));

			svLook *= pTargetTransform->Get_Scale(CTransform::STATE_LOOK);
			svRight *= pTargetTransform->Get_Scale(CTransform::STATE_RIGHT);
			svUp *= pTargetTransform->Get_Scale(CTransform::STATE_UP);

			pTargetTransform->Set_State(CTransform::STATE_RIGHT, svRight);
			pTargetTransform->Set_State(CTransform::STATE_UP, svUp);
			pTargetTransform->Set_State(CTransform::STATE_LOOK, svLook);
			pTargetTransform->Set_State(CTransform::STATE_POSITION, smatResult.r[3]);
		}
	}

	if (m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}

	return _int();
}

_int CSilvermane_Execution::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CSilvermane_Execution::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Execution::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	CActor* pTarget = m_pSilvermane->Get_TargetExecution();
	_int iObjectTag = -1;
	if (pTarget)
	{
		iObjectTag = pTarget->getTag();
	}

	switch (iObjectTag)
	{
	case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
		m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Attack_Execution_Mook_Player", true);
		break;
	case (_uint)GAMEOBJECT::MIDDLE_BOSS:
		m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Attack_Finisher_Phalanxar_Player", true);
		break;
	}
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	return S_OK;
}

HRESULT CSilvermane_Execution::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_Execution(false);
	return S_OK;
}

void CSilvermane_Execution::Hit(const ATTACKDESC& _tAttackDesc)
{
}

_int CSilvermane_Execution::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CSilvermane_Execution* CSilvermane_Execution::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_Execution* pInstance = new CSilvermane_Execution(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_Execution Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_Execution::Free()
{
	__super::Free();
}

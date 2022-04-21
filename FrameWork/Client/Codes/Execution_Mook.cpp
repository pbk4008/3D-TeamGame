#include "pch.h"
#include "Execution_Mook.h"

#include "HierarchyNode.h"

CExecution_Mook::CExecution_Mook(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CSilvermane_Execution(_pDevice, _pDeviceContext)
{
}

HRESULT CExecution_Mook::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CExecution_Mook::Tick(const _double& _dDeltaTime)
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
			_vector svTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

			_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - smatResult.r[3]);
			_vector svRight = XMVector3Normalize(XMVector3Cross(_vector{ 0.f, 1.f, 0.f, 0.f }, svLook));
			_vector svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));

			svLook *= pTargetTransform->Get_Scale(CTransform::STATE_LOOK);
			svRight *= pTargetTransform->Get_Scale(CTransform::STATE_RIGHT);
			svUp *= pTargetTransform->Get_Scale(CTransform::STATE_UP);

			//_float3 vTargetPos{};
			//XMStoreFloat3(&vTargetPos, smatResult.r[3]);
			//pTarget->Set_FootPosition(vTargetPos);
			//pTargetTransform->Set_State(CTransform::STATE_RIGHT, svRight);
			//pTargetTransform->Set_State(CTransform::STATE_UP, svUp);
			//pTargetTransform->Set_State(CTransform::STATE_LOOK, svLook);
			pTargetTransform->Set_State(CTransform::STATE_POSITION, smatResult.r[3]);
		}
	}

	if (m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}

	return _int();
}

_int CExecution_Mook::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CExecution_Mook::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExecution_Mook::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Attack_Execution_Mook_Player", true);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	//m_pSilvermane->Set_Execution(true);
	return S_OK;
}

HRESULT CExecution_Mook::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	//m_pSilvermane->Get_TargetExecution()->Set_Remove(true);
	m_pSilvermane->Set_Execution(false);
	return S_OK;
}

CExecution_Mook* CExecution_Mook::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CExecution_Mook* pInstance = new CExecution_Mook(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CExecution_Mook Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CExecution_Mook::Free()
{
	__super::Free();
}

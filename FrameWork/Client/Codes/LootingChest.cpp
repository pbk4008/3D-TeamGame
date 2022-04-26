#include "pch.h"
#include "LootingChest.h"

#include "DropBox.h"

CLootingChest::CLootingChest(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CLootingChest::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CLootingChest::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pSilvermane->Raycast_DropBox(_dDeltaTime);

	_uint iCurKeyFrameIndex = m_pAnimationController->Get_CurKeyFrameIndex();
	if (68 < iCurKeyFrameIndex)
	{
		CDropBox* pDropBox = m_pSilvermane->Get_TargetDropBox();
		if (pDropBox && !pDropBox->IsOpen())
		{
			pDropBox->Focus();

			CCameraShake::SHAKEEVENT tShakeEvent;
			tShakeEvent.fDuration = 0.7f;
			tShakeEvent.fBlendInTime = 0.1f;
			tShakeEvent.fBlendOutTime = 0.4f;
			//tShakeEvent.tWaveX.fAmplitude = 0.1f;
			//tShakeEvent.tWaveX.fFrequency = 10.f;
			tShakeEvent.tWaveY.fAmplitude = 0.1f;
			tShakeEvent.tWaveY.fFrequency = 1.f;
			tShakeEvent.tWaveY.fAdditionalOffset = 0.4f;
			//tShakeEvent.tWaveZ.fAmplitude = 0.1f;
			//tShakeEvent.tWaveZ.fFrequency = 8.f;

			_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);

			tShakeEvent.fDuration = 0.4f;
			tShakeEvent.tWaveX.fAmplitude = 0.1f;
			tShakeEvent.tWaveX.fFrequency = 10.f;
			XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
			g_pShakeManager->Shake(tShakeEvent, vPos);
		}
	}

	if (!m_pSilvermane->Get_TargetDropBox() || m_pAnimationController->Is_Finished())
	{
		return ToIdle();
	}
	
	return _int();
}

_int CLootingChest::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CLootingChest::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLootingChest::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Looting_Chest_Legendary_Player", false)))
		return E_FAIL;
	m_pAnimationController->Set_RootMotion(true, true);

	CDropBox* pDropBox = m_pSilvermane->Get_TargetDropBox();
	pDropBox->FocusEnter();
	m_pSilvermane->Set_IsBoxOpen(true);

	m_pSilvermane->Set_IsTrasceCamera(false);

	m_pSilvermane->RimlightCheck(true, _float3(0, 0, 1));

	return S_OK;
}

HRESULT CLootingChest::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsBoxOpen(false);

	m_pSilvermane->Set_IsTrasceCamera(true);

	m_pSilvermane->RimlightCheck(false);
	return S_OK;
}

_int CLootingChest::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (!g_pGameInstance->getkeyPress(DIK_C))
	{
		return ToIdle();
	}
	
	return _int();
}

CLootingChest* CLootingChest::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CLootingChest* pInstance = new CLootingChest(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CLootingChest Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLootingChest::Free()
{
	__super::Free();
}

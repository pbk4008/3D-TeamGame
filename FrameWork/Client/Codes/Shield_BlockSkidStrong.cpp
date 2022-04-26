#include "pch.h"
#include "Shield_BlockSkidStrong.h"

CShield_BlockSkidStrong::CShield_BlockSkidStrong(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CShield_Block(_pDevice, _pDeviceContext)
{
}

HRESULT CShield_BlockSkidStrong::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CShield_BlockSkidStrong::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		if (FAILED(m_pStateController->Change_State(L"Shield_BlockLoop")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

_int CShield_BlockSkidStrong::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CShield_BlockSkidStrong::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShield_BlockSkidStrong::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Spectral_Shield_Block_Skid_Strong", false);
	m_pAnimationController->Set_RootMotion(true, true, ERootOption::XYZ);

	m_iCutIndex = 25;
	m_pSilvermane->Set_IsTrasceCamera(false);

	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 0.4f;
	tShakeEvent.fBlendInTime = 0.2f;
	tShakeEvent.fBlendOutTime = 0.1f;
	tShakeEvent.tWaveX.fAmplitude = 0.08f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveY.fAmplitude = 0.08f;
	tShakeEvent.tWaveY.fFrequency = 12.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.08f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;

	_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	g_pShakeManager->Shake(tShakeEvent, vPos);


	return S_OK;
}

HRESULT CShield_BlockSkidStrong::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

_int CShield_BlockSkidStrong::Input(const _double& _dDeltaTime)
{
	_int iProgress = __super::Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CShield_BlockSkidStrong* CShield_BlockSkidStrong::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CShield_BlockSkidStrong* pInstance = new CShield_BlockSkidStrong(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CShield_BlockSkidStrong Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield_BlockSkidStrong::Free()
{
	__super::Free();
}

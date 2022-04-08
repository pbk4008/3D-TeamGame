#include "pch.h"
#include "Silvermane_Death.h"

CSilvermane_Death::CSilvermane_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState_Silvermane(_pDevice, _pDeviceContext)
{
}

HRESULT CSilvermane_Death::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CSilvermane_Death::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pAnimationController->Is_Finished())
	{
		m_pSilvermane->Respawn();
	}

	return _int();
}

_int CSilvermane_Death::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)

		return iProgress;
	return _int();
}

HRESULT CSilvermane_Death::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane_Death::EnterState(void* _pArg)
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	EDir eDir = EDir::Max;
	if(_pArg)
		memcpy_s(&eDir, sizeof(EDir), _pArg, sizeof(EDir));
	switch (eDir)
	{
	case EDir::Forward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Death_Bwd_Player", false)))
			return E_FAIL;
		break;
	case EDir::Backward:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Death_Fwd_Player", false)))
			return E_FAIL;
		break;
	case EDir::Left:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Death_Right_Player", false)))
			return E_FAIL;
		break;
	case EDir::Right:
		if (FAILED(m_pAnimationController->SetUp_NextAnimation("SK_Silvermane.ao|A_Death_Left_Player", false)))
			return E_FAIL;
		break;
	}
	m_pAnimationController->Set_RootMotion(true, true);

	if (!m_isShake)
	{
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 0.4f;
		tShakeEvent.fBlendInTime = 0.1f;
		tShakeEvent.fBlendOutTime = 0.1f;
		tShakeEvent.tWaveX.fAmplitude = 0.04f;
		tShakeEvent.tWaveX.fFrequency = 12.f;
		tShakeEvent.tWaveY.fAmplitude = 0.04f;
		tShakeEvent.tWaveY.fFrequency = 14.f;
		tShakeEvent.tWaveZ.fAmplitude = 0.04f;
		tShakeEvent.tWaveZ.fFrequency = 10.f;

		_float3 vPos; XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		g_pShakeManager->Shake(tShakeEvent, vPos);
		m_isShake = true;
	}

	m_pSilvermane->Set_IsTrasceCamera(false);
	m_pSilvermane->Set_IsDead(true);
	return S_OK;
}

HRESULT CSilvermane_Death::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	m_pSilvermane->Set_IsTrasceCamera(true);
	return S_OK;
}

CSilvermane_Death* CSilvermane_Death::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CSilvermane_Death* pInstance = new CSilvermane_Death(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane_Death Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane_Death::Free()
{
	__super::Free();
}

#include "pch.h"
#include "ShakeManager.h"


CShakeManager::CShakeManager()
{
}

HRESULT CShakeManager::NativeConstruct()
{
	if (FAILED(Ready_Template()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShakeManager::Ready_Template()
{
	CCameraShake::SHAKEEVENT tShakeEvent;
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveX.fAmplitude = 0.1f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	m_arrShakeEvents[(_uint)ETemplate::TestX] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveY.fAmplitude = 0.1f;
	tShakeEvent.tWaveY.fFrequency = 6.f;
	m_arrShakeEvents[(_uint)ETemplate::TestY] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 4.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;
	m_arrShakeEvents[(_uint)ETemplate::TestZ] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveX.fAmplitude = 0.1f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveY.fAmplitude = 0.1f;
	tShakeEvent.tWaveY.fFrequency = 6.f;
	m_arrShakeEvents[(_uint)ETemplate::TestXY] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveX.fAmplitude = 0.1f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;
	m_arrShakeEvents[(_uint)ETemplate::TestXZ] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveY.fAmplitude = 0.1f;
	tShakeEvent.tWaveY.fFrequency = 6.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;
	m_arrShakeEvents[(_uint)ETemplate::TestYZ] = tShakeEvent;


	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 4.f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveX.fAmplitude = 0.1f;
	tShakeEvent.tWaveX.fFrequency = 10.f;
	tShakeEvent.tWaveY.fAmplitude = 0.1f;
	tShakeEvent.tWaveY.fFrequency = 6.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 8.f;
	m_arrShakeEvents[(_uint)ETemplate::TestXYZ] = tShakeEvent;

	tShakeEvent = CCameraShake::SHAKEEVENT();
	tShakeEvent.fDuration = 0.5f;
	tShakeEvent.fOuterRadius = 10.f;
	tShakeEvent.fInnerRadius = 3.f;
	tShakeEvent.tWaveX.fAmplitude = 0.1;
	tShakeEvent.tWaveX.fFrequency = 3.f;
	tShakeEvent.tWaveY.fAmplitude = 0.02f;
	tShakeEvent.tWaveY.fFrequency = 3.f;
	tShakeEvent.tWaveZ.fAmplitude = 0.1f;
	tShakeEvent.tWaveZ.fFrequency = 3.f;
	m_arrShakeEvents[(_uint)ETemplate::MonsterDeath] = tShakeEvent;

	return S_OK;
}

void CShakeManager::setTemplate(const ETemplate _eTemplate, const CCameraShake::SHAKEEVENT& _tShakeEvent)
{
	m_arrShakeEvents[(_uint)_eTemplate] = _tShakeEvent;
}

void CShakeManager::setOriginCameraShake(CCameraShake* _pOrigin)
{
	m_pOrigin = _pOrigin;
}

void CShakeManager::Shake(const CCameraShake::SHAKEEVENT& _tShakeEvent, const _float3& _vEventPos)
{
	if (!m_pOrigin)
		return;
	m_pOrigin->Shake(_tShakeEvent, _vEventPos);
}

void CShakeManager::Shake(const CCameraShake::SHAKEEVENT& _tShakeEvent, const _fvector& _svEventPos)
{
	if (!m_pOrigin)
		return;

	_float3 vEventPos;
	XMStoreFloat3(&vEventPos, _svEventPos);
	m_pOrigin->Shake(_tShakeEvent, vEventPos);
}

void CShakeManager::Shake(const ETemplate _eTemplate, const _float3& _vEventPos)
{
	if (!m_pOrigin)
		return;
	m_pOrigin->Shake(m_arrShakeEvents[(_uint)_eTemplate], _vEventPos);
}

void CShakeManager::Shake(const ETemplate _eTemplate, const _fvector& _svEventPos)
{
	if (!m_pOrigin)
		return;

	_float3 vEventPos;
	XMStoreFloat3(&vEventPos, _svEventPos);
	m_pOrigin->Shake(m_arrShakeEvents[(_uint)_eTemplate], vEventPos);
}

void CShakeManager::StopShake()
{
	if (!m_pOrigin)
		return;
	m_pOrigin->StopShake();
}

void CShakeManager::Free()
{
}

#include "pch.h"
#include "CameraShake.h"

#include "Camera_Silvermane.h"

CCameraShake::CCameraShake(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
{
}

CCameraShake::CCameraShake(const CCameraShake& _rhs)
	: CComponent(_rhs)
{
}

HRESULT CCameraShake::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCameraShake::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	g_pShakeManager->setOriginCameraShake(this);

	return S_OK;
}

const _int CCameraShake::Tick(CGameObject* _pCamera, const _double& _dDeltaTime)
{
	PlayShake(_pCamera, _dDeltaTime);

	return _int();
}

void CCameraShake::PlayShake(CGameObject* _pCamera, const _double& _dDeltaTime)
{
	if (0 == m_vecShakeEvents.size())
	{
		m_isShaking = false;
		return;
	}
	else
		m_isShaking = true;

	CCamera_Silvermane* pCamera_Silvermane = static_cast<CCamera_Silvermane*>(_pCamera);
	CTransform* pTargetWorldTransform = pCamera_Silvermane->m_pTransform;
	//CTransform* pTargetLocalTransform = pCamera_Silvermane->m_pLocalTransform;

	_float3 vAccPos = { 0.f, 0.f, 0.f };
	_float3 vAccRot = { 0.f, 0.f, 0.f };
	_float3 vCamPos;
	_float3 vCamLocalOriginPos;
	XMStoreFloat3(&vCamPos, pTargetWorldTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat3(&vCamLocalOriginPos, XMLoadFloat3(&pCamera_Silvermane->m_vLocalOriginPos));

	for (auto iter = m_vecShakeEvents.begin(); iter != m_vecShakeEvents.end();)
	{
		(*iter).fTimer += _float(_dDeltaTime) * (*iter).fShakeSpeed;

		(*iter).CalcDistanceRate(vCamPos);
		if ((*iter).fTimer < (*iter).fBlendInTime)
		{
			(*iter).fAmplitudeRate = (*iter).fTimer / (*iter).fBlendInTime;
		}
		else if ((*iter).fTimer > (*iter).fDuration - (*iter).fBlendOutTime)
		{
			if ((*iter).fBlendOutTime != 0.f)
				(*iter).fAmplitudeRate = ((*iter).fDuration- (*iter).fTimer) / (*iter).fBlendOutTime;
			else
				(*iter).fAmplitudeRate = 0.f;
		}
		else
		{
			(*iter).fAmplitudeRate = 1.f;
		}

		(*iter).fAmplitudeRate *= (*iter).fDistanceRate;

		vAccRot.x += (*iter).AdvanceSinWave((*iter).tPitchWave);
		vAccRot.y += (*iter).AdvanceSinWave((*iter).tYawWave);
		vAccRot.z += (*iter).AdvanceSinWave((*iter).tRollWave);

		vAccPos.x += (*iter).AdvanceSinWave((*iter).tWaveX);
		vAccPos.y += (*iter).AdvanceSinWave((*iter).tWaveY);
		vAccPos.z += (*iter).AdvanceSinWave((*iter).tWaveZ);

		if ((*iter).fTimer > (*iter).fDuration)
			iter = m_vecShakeEvents.erase(iter);
		else
			++iter;
	}

	//D3DXQuaternionRotationYawPitchRoll(&m_transform->rotation, vAccRot.y, vAccRot.x, vAccRot.z);
	//pTargetLocalTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&vCamLocalOriginPos) + XMLoadFloat3(&vAccPos), 1.f));
	pCamera_Silvermane->m_vShakeAccPos = vAccPos;
}

const _bool CCameraShake::IsShaking() const
{
	return m_isShaking;
}

void CCameraShake::Shake(const SHAKEEVENT& _tShakeEvent, const _float3& _vEventPos)
{
	// 굳이 주소값으로 받는 이유는 넘어오는 중간에 복사가 일어나지 않도록 하기 위함.
	// 근디 어차피 안에서 복사하기때문에 밖에서 임시변수 넣어도 괜찮다고 함 -> 그래서 원래 point 사용하던 것을 const 래퍼런스로 바꿈
	SHAKEEVENT tShakeEvent(_vEventPos);
	tShakeEvent = _tShakeEvent;
	tShakeEvent.vEventPos = _vEventPos;

	m_vecShakeEvents.emplace_back(tShakeEvent);
}

void CCameraShake::StopShake()
{
	m_vecShakeEvents.clear();
}

CCameraShake* CCameraShake::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCameraShake* pInstance = new CCameraShake(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCameraShake Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCameraShake::Clone(void* _pArg)
{
	CCameraShake* pInstance = new CCameraShake(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CCameraShake Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCameraShake::Free()
{
	m_vecShakeEvents.clear();

	__super::Free();
}

CCameraShake::tagShakeEvent::tagShakeEvent(const _float3& _vEventPos)
{
	vEventPos = _vEventPos;
}

CCameraShake::tagShakeEvent::tagShakeEvent()
{
	vEventPos = { 0.f, 0.f, 0.f };
}

void CCameraShake::tagShakeEvent::Reset()
{
	fAmplitudeRate = 1.f;
	fTimer = 0.f;

	fDuration = 0.f;
	fBlendInTime = 0.f;
	fBlendOutTime = 0.1f;

	/* For.Rotation */
	tPitchWave.fAmplitude = 0.f;
	tPitchWave.fFrequency = 0.f;
	tPitchWave.fOffset = 0.f;
	tYawWave.fAmplitude = 0.f;
	tYawWave.fFrequency = 0.f;
	tYawWave.fOffset = 0.f;
	tRollWave.fAmplitude = 0.f;
	tRollWave.fFrequency = 0.f;
	tRollWave.fOffset = 0.f;

	/* For.Move */
	tWaveX.fAmplitude = 0.f;
	tWaveX.fFrequency = 0.f;
	tWaveX.fOffset = 0.f;
	tWaveY.fAmplitude = 0.f;
	tWaveY.fFrequency = 0.f;
	tWaveY.fOffset = 0.f;
	tWaveZ.fAmplitude = 0.f;
	tWaveZ.fFrequency = 0.f;
	tWaveZ.fOffset = 0.f;
}

_float CCameraShake::tagShakeEvent::AdvanceSinWave(WAVE _tWave)
{
	if (0.f == _tWave.fAmplitude)
		return 0.f;

	_float fSinFrequency = 2.f * XM_PI * _tWave.fFrequency;
	_float fCurTimeLine = fSinFrequency * (fTimer + _tWave.fOffset);

	return (sinf(fCurTimeLine) * _tWave.fAmplitude + _tWave.fAdditionalOffset) * fAmplitudeRate;
}

void CCameraShake::tagShakeEvent::CalcDistanceRate(const _float3& _vCamPos)
{
	_vector svLen = XMVector3Length(XMLoadFloat3(&vEventPos) - XMLoadFloat3(&_vCamPos));
	_float fLen = XMVectorGetX(svLen);

	if (fLen > fOuterRadius)
		fDistanceRate = 0.f;
	else if (fLen > fInnerRadius)
		fDistanceRate = 1 - ((fLen - fInnerRadius) / (fOuterRadius - fInnerRadius));
	else
		fDistanceRate = 1.f;
}

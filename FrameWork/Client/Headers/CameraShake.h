#pragma once

#include "Component.h"

BEGIN(Client)

class CCameraShake : public CComponent
{
public:
	typedef struct tagWave
	{
		_float fAmplitude = 0.f; // ���� = ����
		_float fFrequency = 0.f; // ���ļ� = Ƚ��.
		_float fOffset = 0.f; // ������ġ = sign �׷��������� ��������.
		_float fAdditionalOffset = 0.f;
		// sign �׷��� ��ü�� ������. �ƿ� y���� ����ġ�� �÷�������.
		// => �������θ� �ö󰡰ų� �����θ� ���������ϴ� �����϶� ���.
	}WAVE;
	typedef struct tagShakeEvent
	{
		tagShakeEvent(const _float3& _vEventPos);
		tagShakeEvent();

		_float fTimer = 0.f; // ElapsedTime

		_float fDuration = 0.5f; // �ð�
		_float fBlendInTime = 0.f; // FadeIn Time
		_float fBlendOutTime = 0.f; // FadeOut Time

		_float fShakeSpeed = 1.f;

		_float fInnerRadius = 5.f; // ����ũ�� ���Ұ� ���۵Ǵ� ���� ����
		_float fOuterRadius = 10.f; // �ִ� ���� ����(�� ���ķδ� ������ �ȹ���)
		_float fDistanceRate = 1.f; //

		_float fAmplitudeRate = 1.f; // ������ ����

		/* For.Rotate Shake */
		WAVE tPitchWave;
		WAVE tYawWave;
		WAVE tRollWave;

		/* For.Move Shake */
		WAVE tWaveX;
		WAVE tWaveY;
		WAVE tWaveZ;

		_float3 vEventPos; // �߻��� ����

		void Reset();
		_float AdvanceSinWave(WAVE _tWave);
		void CalcDistanceRate(const _float3& _vCamPos);
	}SHAKEEVENT;
private:
	explicit CCameraShake(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCameraShake(const CCameraShake& _rhs);
	virtual ~CCameraShake() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	const _int Tick(CGameObject* _pCamera, const _double& _dDeltaTime);

private:
	void PlayShake(CGameObject* _pCamera, const _double& _dDeltaTime);

public:
	void Shake(SHAKEEVENT* _tShakeEvent, const _float3& _vEventPos);
	void StopShake();
	
private:
	vector<SHAKEEVENT> m_vecShakeEvents;

public: /* For.Test */
	SHAKEEVENT m_tTestShakeEvent;
	void ShakeTest();
	void Shaking_TestEvent(const _float3& _vPos = { 0.f, 0.f, 0.f });

public:
	static CCameraShake* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
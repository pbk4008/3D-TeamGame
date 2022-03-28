#pragma once

#include "Component.h"

BEGIN(Client)

class CCameraShake : public CComponent
{
public:
	typedef struct tagWave
	{
		_float fAmplitude = 0.f; // 진폭 = 세기
		_float fFrequency = 0.f; // 주파수 = 횟수.
		_float fOffset = 0.f; // 시작위치 = sign 그래프에서의 시작지점.
		_float fAdditionalOffset = 0.f;
		// sign 그래프 자체의 오프셋. 아예 y축의 기준치를 올려버린다.
		// => 위쪽으로만 올라가거나 밑으로만 내려가야하는 연출일때 사용.
	}WAVE;
	typedef struct tagShakeEvent
	{
		tagShakeEvent(const _float3& _vEventPos);
		tagShakeEvent();

		_float fTimer = 0.f; // ElapsedTime

		_float fDuration = 0.5f; // 시간
		_float fBlendInTime = 0.f; // FadeIn Time
		_float fBlendOutTime = 0.f; // FadeOut Time

		_float fShakeSpeed = 1.f;

		_float fInnerRadius = 5.f; // 쉐이크의 감소가 시작되는 영향 범위
		_float fOuterRadius = 10.f; // 최대 영향 범위(이 이후로는 영향을 안받음)
		_float fDistanceRate = 1.f; //

		_float fAmplitudeRate = 1.f; // 세기의 배율

		/* For.Rotate Shake */
		WAVE tPitchWave;
		WAVE tYawWave;
		WAVE tRollWave;

		/* For.Move Shake */
		WAVE tWaveX;
		WAVE tWaveY;
		WAVE tWaveZ;

		_float3 vEventPos; // 발생할 지점

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
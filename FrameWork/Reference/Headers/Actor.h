#pragma once

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CActor abstract : public CGameObject
{
protected:
	explicit CActor();
	explicit CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CActor(const CActor& rhs);
	virtual ~CActor() = default;
protected:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype();
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	const _float Get_HpRatio();
	const _float Get_GroggyGaugeRatio();
protected:
	virtual HRESULT Set_SpawnPosition(const _float3 vPoint);
	virtual HRESULT Set_SpawnPosition(_fvector vPos);
public:
	virtual HRESULT BindConstantBuffer(const wstring & camTag,SCB* bindbuffer = nullptr, RIM* rimbuffer = nullptr, MOTIONBLUR* motionbuffer = nullptr);
	virtual HRESULT	BindLightBuffer();

public:
	void Active_Effect(_uint iEffectIndex);
	//Effect 좌표받아와서 불러오는 함수
	void Active_Effect(_uint iEffectIndex, _fvector vPivot);
	void Active_Effect_Target(_uint iEffectIndex, _matrix TargetMat);
	void Check_NoDamage(_double dDeltaTime);
public:
	const _bool Get_Dead() { return m_bDead; }
	_float Get_CurrentHp() { return m_fCurrentHp; }
	_float Get_MaxHp() { return m_fMaxHp; }
	_float Get_GroggyGauge() { return m_fGroggyGauge; }
	_float Get_MaxGroggyGauge() { return m_fMaxGroggyGauge; }
	const _bool IsAttack() { return m_IsAttack; }
	_bool Get_Groggy() { return m_bGroggy; }
	const ATTACKDESC& Get_AttackDesc() const;
	_bool Get_NoDamage() { return m_isNoDamage; }
public:
	virtual void Set_IsAttack(const _bool _isAttack) { m_IsAttack = _isAttack; }
	void Set_CurrentHp(_float fCurrentHp) { m_fCurrentHp = fCurrentHp; }
	void Set_GroggyGage(_float fGauge) { m_fGroggyGauge = fGauge; }
	void Set_MaxHp(_float fMaxHp) { m_fMaxHp = fMaxHp; }
	void Set_Groggy(const _bool bGroggy) { m_bGroggy = bGroggy; }
	void Set_UIShow(_bool bShow) { m_bUIShow = bShow; }

	void Set_AttackDesc(const ATTACKDESC& _tAttackDesc);
	void Set_AttackDesc_Object(CGameObject* pObj) { m_tAttackDesc.pOwner = pObj; }
	void Set_AttackDesc_HitObject(CGameObject* pObj) { m_tAttackDesc.pHitObject = pObj; }
	void Set_AttackDesc_Damaga(_float fDamage) { m_tAttackDesc.fDamage = fDamage; }
	void Set_AttackDesc_Level(_uint iLevel) { m_tAttackDesc.iLevel = iLevel; }
	void Set_AttackDesc_Dir(const EAttackDir _eDir) { m_tAttackDesc.eDir = _eDir; }

	void Set_NoDamage(_bool bCheck) { m_isNoDamage = bCheck; }

	virtual void Set_FootPosition(const _float3& _vPos);

	void RimlightCheck(_bool check, _float3 color = _float3(0,0,0));
	_bool GetRimCheck() { return m_rimcheck; }
	void SetRimIntensity(_float time);
	
	void Set_Dissolve(_bool on) { m_bdissolve = on; }
	HRESULT DissolveOn(_float dissolveSpeed = 1.f);

public:
	virtual void Hit(const ATTACKDESC& _tAttackDesc);
	virtual void Parry(const PARRYDESC& _tParryDesc);
	virtual void Execution();
	virtual void Execution(CActor* _pOther, CHierarchyNode* _pFixedBone);

protected:
	virtual void Free() override;
protected:
	class CModel* m_pModel = nullptr;
	_bool m_bDead;//죽었는지 안죽었는지 판단
	_bool m_IsAttack;//공격했는지 공격 안했는지
	_bool m_bGroggy; //스턴상태인지 아닌지
	_bool m_bUIShow = false; //몬스터머리위에 ui보이는상태인지아닌지
	_bool m_isParry = false; // 패링상태 체크
	_bool m_isExecution = false; // 처형당하는 중인지 체크
	_bool m_isNoDamage = false;// 무적상태인지 안닌지 체크

	_float m_fSpeed;//이동 속도
	_float m_fMaxHp;//최대 체력
	_float m_fCurrentHp;//현재 체력
	_float m_fGroggyGauge; //스턴게이지
	_float m_fMaxGroggyGauge; //스턴게이지
	_float m_fAccNoDamageTime;//무적상태 Time

	ATTACKDESC m_tAttackDesc;
	const LIGHTDESC* m_lightdesc;
	
	// dissovle
	_bool			m_bdissolve = false;
	_float			m_lifetime = 0.f;
	CTexture*		m_dissolveTex = nullptr;
	CTexture*		m_dissolveGradientTex = nullptr;

	// rim light
	_bool			m_rimcheck = false;
	_float			m_rimintensity = 30.f;
	_float3			m_rimcol = _float3(0, 0, 0);

	// motion blur
	_bool			m_motionblurcheck = false;
	_float			m_timer = 0.f;
};
END
#endif
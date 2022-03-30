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
	/* ������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct_Prototype();
	/* ��������ü�� �����ɶ� ȣ��Ǵ� �Լ�. */
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	const _float Get_HpRatio();
	const _float Get_GroggyGaugeRatio();
protected:
	HRESULT Set_SpawnPosition(const _float3 vPoint);
public:
	const _bool Get_Dead() { return m_bDead; }
	_float Get_CurrentHp() { return m_fCurrentHp; }
	_float Get_MaxHp() { return m_fMaxHp; }
	_float Get_GroggyGauge() { return m_fGroggyGauge; }
	const _bool IsAttack() { return m_IsAttack; }
	_bool Get_Groggy() { return m_bGroggy; }
public:
	void Set_CurrentHp(_float fCurrentHp) { m_fCurrentHp = fCurrentHp; }
	void Set_GroggyGage(_float fGauge) { m_fGroggyGauge = fGauge; }
	void Set_MaxHp(_float fMaxHp) { m_fMaxHp = fMaxHp; }
	virtual void Set_IsAttack(const _bool _isAttack) { m_IsAttack = _isAttack; }
	void Set_Groggy(const _bool bGroggy) { m_bGroggy = bGroggy; }
	void Set_UIShow(_bool bShow) { m_bUIShow = bShow; }

protected:
	virtual void Free() override;
protected:
	_bool m_bDead;//�׾����� ���׾����� �Ǵ�
	_bool m_IsAttack;//�����ߴ��� ���� ���ߴ���
	_bool m_bGroggy;//���ϻ������� �ƴ���
	_bool m_bUIShow = false; //���͸Ӹ����� ui���̴»��������ƴ���

	_float m_fSpeed;//�̵� �ӵ�
	_float m_fMaxHp;//�ִ� ü��
	_float m_fCurrentHp;//���� ü��
	_float m_fGroggyGauge; //���ϰ�����
	_float m_fMaxGroggyGauge; //���ϰ�����

};
END
#endif
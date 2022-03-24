#pragma once

#ifndef UI_Monster_Panel_h__
#define UI_Monster_Panel_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_Panel : public CUI
{
public:
	enum class Enemy
	{
		CRAWLER, ABERRANT ,ANIMUS, SWORD, SPEAR, SHOOTER, HEALER, SWORD2H, MIDBOSS, ENDBOSS, ENEMY_END
	};

public:
	typedef struct tagUIDesc
	{
		CTransform* pTargetTransform;
		Enemy iEnemyTag;
	}PANELDESC;

protected:
	explicit CUI_Monster_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_Panel(const CUI& rhs);
	virtual ~CUI_Monster_Panel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_TargetWorldMatrix(_matrix matTarget) { XMStoreFloat4x4(&m_TargetMatrix, matTarget);}

public:
	void Set_HpBar(_float fRatio);
	void Set_GroggyBar(_float fRatio);
	void Set_BackUIGapY(_float GapY);

private:
	HRESULT Panel_Setting();
	HRESULT Setting_Crawler();
	HRESULT Setting_Aberrant();
	HRESULT Setting_Animus();
	HRESULT Setting_Sword();
	HRESULT Setting_Spear();
	HRESULT Setting_Shooter();
	HRESULT Setting_Healer();
	HRESULT Setting_2HSword();
	HRESULT Setting_MidBoss();
	HRESULT Setting_EndBoss();

private:
	void Update_Panel(_double TimeDelta);
	void Update_Setting_Crawler(_double TimeDelta);
	void Update_Setting_Aberrant(_double TimeDelta);
	void Update_Setting_Animus(_double TimeDelta);
	void Update_Setting_Sword(_double TimeDelta);
	void Update_Setting_Spear(_double TimeDelta);
	void Update_Setting_Shooter(_double TimeDelta);
	void Update_Setting_Healer(_double TimeDelta);
	void Update_Setting_2HSword(_double TimeDelta);
	void Update_Setting_MidBoss(_double TimeDelta);
	void Update_Setting_EndBoss(_double TimeDelta);

private:
	PANELDESC m_PanelDesc;
	_float4x4 m_TargetMatrix;
	Enemy m_EnemyTag = Enemy::ENEMY_END;

	class CUI_Monster_Back* m_pUIBack = nullptr;
	class CUI_Monster_Level* m_pUILevel = nullptr;
	class CUI_Monster_HpBar* m_pUIHpBar = nullptr;
	class CUI_Monster_Name* m_pUIName = nullptr;
	class CUI_Monster_GroggyBar* m_pUIGroggyBar = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_Panel_h__

#pragma once

#ifndef UI_Monster_HpBar_h__
#define UI_Monster_HpBar_h__

#include "UI_Monster_Panel.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_HpBar final : public CUI_Monster_Panel
{
public:
	typedef struct UIBar
	{
		CUI::UIDESC UIDesc;
		_uint iRenderPass;
	}UIBARDESC;
private:
	explicit CUI_Monster_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_HpBar(const CUI_Monster_Panel& rhs);
	virtual ~CUI_Monster_HpBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIBARDESC Desc) { m_UIBarDesc = Desc; }
	UIBARDESC Get_UIDesc() { return m_UIBarDesc; }

public:
	void Set_TargetHpBar(_float fMaxHp, _float fHp);

private:
	UIBARDESC m_UIBarDesc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;

	_float m_fTargetMaxHp = 0.f;
	_float m_fTargetHp = 0.f;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_HpBar_h__

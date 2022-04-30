#pragma once

#ifndef UI_Monster_HpBar_h__
#define UI_Monster_HpBar_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_HpBar final : public CUI
{
public:
	typedef struct UIBar
	{
		CUI::UIDESC UIDesc;
		_uint iRenderPass;
	}UIBARDESC;
private:
	explicit CUI_Monster_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_HpBar(const CUI_Monster_HpBar& rhs);
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
	void Set_ExHpRatio(_float Ratio) { m_fExHpRatio = Ratio; }
	void Set_HpRatio(_float Ratio) { m_fHpRatio = Ratio; }

private:
	UIBARDESC m_UIBarDesc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;


	_bool m_bAutoDis = false; //�ڵ����β������ְ�
	_float m_fAutoDisTimeAcc = 0.f; //�ڵ����β����ð�

	_float m_fExHpRatio = 0.f; //���� �� ����
	_float m_fHpRatio = 0.f; //���� �� ����
	_float m_fCurAttackGauge = 0.f; //���������� - ���������

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_HpBar_h__

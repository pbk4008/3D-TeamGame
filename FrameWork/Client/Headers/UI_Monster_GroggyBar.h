#pragma once

#ifndef UI_Monster_GroggyBar_h__
#define UI_Monster_GroggyBar_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_GroggyBar final : public CUI
{
public:
	typedef struct UIBar
	{
		CUI::UIDESC UIDesc;
		_uint iRenderPass;
	}UIBARDESC;
private:
	explicit CUI_Monster_GroggyBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_GroggyBar(const CUI_Monster_GroggyBar& rhs);
	virtual ~CUI_Monster_GroggyBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIBARDESC Desc) { m_UIBarDesc = Desc; }
	UIBARDESC Get_UIDesc() { return m_UIBarDesc; }

	void Set_GroggyRatio(_float Ratio) { m_fGroggyRatio = Ratio; }

private:
	UIBARDESC m_UIBarDesc;
	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;
	_float m_fAlpha = 1.f;
	_float m_fDisappearTimeAcc = 0.f;


	_bool m_bAutoDis = false; //자동으로꺼질수있게
	_float m_fAutoDisTimeAcc = 0.f; //자동으로꺼질시간

	_float m_fGroggyRatio = 0.f;

private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_GroggyBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_GroggyBar_h__

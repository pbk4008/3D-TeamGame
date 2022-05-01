#pragma once

#ifndef UI_Monster_BackHpBar_h__
#define UI_Monster_BackHpBar_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_BackHpBar final : public CUI
{
public:
	typedef struct UIBar
	{
		CUI::UIDESC UIDesc;
		_uint iRenderPass;
	}UIBARDESC;
private:
	explicit CUI_Monster_BackHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_BackHpBar(const CUI_Monster_BackHpBar& rhs);
	virtual ~CUI_Monster_BackHpBar() = default;

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
	void Set_HpRatio(_float Ratio) { m_fHpRatio = Ratio; }

private:
	UIBARDESC m_UIBarDesc;
	_float m_fHpRatio = 0.f;
	_float m_fExHpRatio = 0.f; //이전 피 비율

	_float m_fGapX = 0.f;
	_float m_fGapY = 0.f;
	_float m_fAlpha = 1.f; //레이닿았을때 사라지면서 줄어들 alpha
	_float m_fDisappearTimeAcc = 0.f; //레이닿았을때 사라지는 시간체크 

	_bool m_bAutoDis = false; //자동으로꺼질수있게
	_float m_fAutoDisTimeAcc = 0.f; //자동으로꺼질시간

	_bool m_bChange = false; //알파값이 줄어들기 시작해야하는 타이밍 체크해줌 
	_float m_fRatioAlpha = 1.f; //피 깎였을때 보여야하는 알파값


private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_BackHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_BackHpBar_h__

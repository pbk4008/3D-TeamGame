#pragma once
#ifndef UI_LevelHUD_h__
#define UI_LevelHUD_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_LevelHUD : public CUI
{
	struct Desc
	{

	};

	explicit CUI_LevelHUD(void) = default;
	explicit CUI_LevelHUD(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_LevelHUD(const CUI_LevelHUD& rhs);
	virtual ~CUI_LevelHUD() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetImage(std::wstring szTexName);
	void	FadeOut(void);
	void	ResetAlpha(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc desc;

public:
	static CUI_LevelHUD* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_LevelHUD_h__

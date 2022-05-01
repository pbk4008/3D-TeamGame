#pragma once
#ifndef UI_MapInfo_h__
#define UI_MapInfo_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CMapInfo : public CUI
{
public:
	struct Desc
	{

	};

	explicit CMapInfo(void) = default;
	explicit CMapInfo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CMapInfo(const CMapInfo & rhs);
	virtual ~CMapInfo() = default;

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

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc desc;

public:
	static CMapInfo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;

};
END

#endif // UI_MapInfo_h__
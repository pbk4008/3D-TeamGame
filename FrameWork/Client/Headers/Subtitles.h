#pragma once
#ifndef Subtitles_h__
#define Subtitles_h__

#include "UI.h"

BEGIN(Engine)
class CSoundMgr;
END

BEGIN(Client)
class CSingleImage;
class CSubtitles :public CUI
{
public:
	struct Desc
	{
		_float3 fPos;
		_uint	iNumTex;
		CRenderer::RENDER ERenderGroup = CRenderer::RENDER::RENDER_UI;
		CSoundMgr::CHANNELID EChannel_1;
		CSoundMgr::CHANNELID EChannel_2;
		_bool bUsingCinema=false;
	};

	explicit CSubtitles(void) = default;
	explicit CSubtitles(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CSubtitles(const CSubtitles& rhs);
	virtual ~CSubtitles() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetImage(std::wstring szTexName);
	_uint	GetNumTex(void) { return desc.iNumTex; }
	void	UpScaleing(void);
	void	DownScaleing(void);
private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CTransform*	  m_pTriggerTrans = nullptr;
	CGameObject*  m_pPlayer = nullptr;

private:
	_int m_interactDist = 10;
	Desc desc;
private:
	_bool m_bUsingCinema=false;
public:
	static CSubtitles* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;

};

END
#endif // Subtitles_h__


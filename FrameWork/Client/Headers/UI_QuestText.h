#ifndef UI_QuestText_h__
#define UI_QuestText_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_QuestText : public CUI
{
public:
	struct Desc
	{
		EQuestTextType	 EQuestText;
		CUI* pOwner = nullptr;
	};

	explicit CUI_QuestText(void) = default;
	explicit CUI_QuestText(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_QuestText(const CUI_QuestText& rhs);
	virtual ~CUI_QuestText() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);
	void SetIcon(std::wstring _szTextureName);
	void SetPosy(_float fPosy);
	void SetFadeOut(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	Desc		  desc;
	_float		  m_fPosY = 180.f;
	_bool		  m_bOffsetY = false;
public:
	static CUI_QuestText* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_QuestText_h__
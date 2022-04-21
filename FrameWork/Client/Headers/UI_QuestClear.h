#ifndef UI_QuestClear_h__
#define UI_QuestClear_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_QuestClear : public CUI
{
public:
	struct Desc
	{
		CUI*			 pOwner = nullptr;
	};

	explicit CUI_QuestClear(void) = default;
	explicit CUI_QuestClear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_QuestClear(const CUI_QuestClear& rhs);
	virtual ~CUI_QuestClear() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);
	void Show(_double dTimeDelta);
	void SetIcon(std::wstring _szTextureName);
	void SetPosy(_float fPosy);
	void SetFadeOut(void);
private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	Desc		  desc;
	_float		  m_fPosY = 180.f;
	_float2		  m_fInitScale = { 120.f, 120.f };
	_float2		  m_fEndScale = { 30.5f , 30.5f };
	_bool		  m_bShowEnd = false;

public:
	static CUI_QuestClear* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END


#endif // UI_QuestClear_h__

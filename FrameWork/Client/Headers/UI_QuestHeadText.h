#ifndef UI_QuestHeadText_h__
#define UI_QuestHeadText_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_QuestHeadText : public CUI
{
public:
	struct Desc
	{
		EQuestHeaderType  EHeadText;
	};

	explicit CUI_QuestHeadText(void) = default;
	explicit CUI_QuestHeadText(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_QuestHeadText(const CUI_QuestHeadText& rhs);
	virtual ~CUI_QuestHeadText() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void SetIcon(std::wstring _szTextureName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUI*		  m_pOwner = nullptr;
	Desc		  desc;

public:
	static CUI_QuestHeadText* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_QuestHeadText_h__
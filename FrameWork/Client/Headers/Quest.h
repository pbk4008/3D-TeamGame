#ifndef Quest_h__
#define Quest_h__

#include "UI.h"

BEGIN(Client)
class CUI_QuestText;
class CUI_QuestClear;
class CQuest final : public CUI
{
public:
	struct Desc
	{
		EQuestTextType	 EQuestText;
	};

public:
	explicit CQuest(void) = default;
	explicit CQuest(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CQuest(const CUI& rhs);
	virtual ~CQuest() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Component(void);
	virtual HRESULT Ready_UIObject(void* pArg);

public:
	std::wstring GetQuestName(void) { return m_szQuestText; }
	void SetIndex(_int Index)		{ m_iIndex = Index; }
	void ActiveQuestClear(_bool active);
	void Pulling(_double dDeltaTime);
	_bool IsAlive(void) { return m_bAlive; }
	void SetFadeOutAll(void);
	void SetPosUp(void);
private:
	CUI_QuestText*	m_pQuestText = nullptr;
	CUI_QuestClear* m_pQuestClear = nullptr;

private:
	std::wstring	m_szQuestText = L"";
	_int			m_iIndex;
	_float			m_fPosY = 180.f;
	_bool			m_bSetY = false;
	_bool			m_bBye  = false;
	_bool			m_bAlive = true;
	_bool			m_bPosUp = false;

private:
	_float			m_fDisaperXInitPos = 0.f;
	_float			m_fDisaperXEndPos  = -700.f;
	_float			m_fDisapearTime = 0.f;
	_float			m_fUpYInitPos = 0.f;
	_float			m_fUpYEndPos = 30.f;

public:
	static  CQuest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg) override;

private:
	virtual void Free() override;
};

END
#endif // Quest_h__

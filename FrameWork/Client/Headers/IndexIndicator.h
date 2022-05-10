#ifndef IndexIndicator_h__
#define IndexIndicator_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUIHelper;
class CIndexIndicator final : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner;
	};
	explicit CIndexIndicator(void) = default;
	explicit CIndexIndicator(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CIndexIndicator(const CIndexIndicator& rhs);
	virtual ~CIndexIndicator() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_bool   ButtonClicked(void);
	void	SetMove(_bool MoveOn) { m_bMove = MoveOn; }
	void	SetInitEquipXPos(_float fPos) { 
		m_fInitPos = m_fEndPos;
		m_fEndPos = fPos;
	}
	void	SetInitArmoryXPos(_float fPos) {
		m_fInitPos = m_fEndPos;
		m_fEndPos = fPos;
	}
	void   ResetXPos(void);

private:
	void	MoveLeft(_double TimeDelta);
	void	MoveRight(_double TimeDelta);

public:
	static CIndexIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUIHelper* m_pUIHelperCom = nullptr;
	Desc desc;

private:
	_bool m_bMove = false;

	_float m_fInitPos = -820.f;
	_float m_fEndPos  = -720.f;

private:
	virtual void Free() override;
};

END

#endif // IndexIndicator_h__
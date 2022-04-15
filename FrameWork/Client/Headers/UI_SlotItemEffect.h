#ifndef UI_SlotItemEffect_h__
#define UI_SlotItemEffect_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_SlotItemEffect :public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

public:
	explicit CUI_SlotItemEffect(void) = default;
	explicit CUI_SlotItemEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_SlotItemEffect(const CUI_SlotItemEffect& rhs);
	virtual ~CUI_SlotItemEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int	Attach_Owner(void);

public:
	void	SetIcon(const std::wstring & _szFileName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	ItemSlotDesc  desc;
	CTransform*	  m_pLocalTransform = nullptr;
	CUI*		  m_pOwner = nullptr;

public:
	static CUI_SlotItemEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_SlotItemEffect_h__


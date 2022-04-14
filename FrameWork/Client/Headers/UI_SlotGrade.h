#ifndef UI_SlotGrade_h__
#define UI_SlotGrade_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_SlotGrade final : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI* pOwner = nullptr;
	};

	explicit CUI_SlotGrade(void) = default;
	explicit CUI_SlotGrade(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_SlotGrade(const CUI_SlotGrade& rhs);
	virtual ~CUI_SlotGrade() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetGrade(std::wstring _szTextureName);
	_int	Attach_Owner(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	ItemSlotDesc  desc;
	CTransform*	  m_pLocalTransform = nullptr;
	CUI*		  m_pOwner = nullptr;

public:
	static CUI_SlotGrade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};
END
#endif // UI_SlotGrade_h__

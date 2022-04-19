#ifndef UI_LootBackground_h__
#define UI_LootBackground_h__

#include "Hud.h"

BEGIN(Client)
class CSingleImage;
class CUI_LootBackground : public CHud
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_LootBackground(void) = default;
	explicit CUI_LootBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_LootBackground(const CUI_LootBackground& rhs);
	virtual ~CUI_LootBackground() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);

public:
	void SetBg(const std::wstring& _szFileName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CHud::Desc    desc;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	_bool		  m_bSetScale = true;

public:
	static CUI_LootBackground* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_LootBackground_h__

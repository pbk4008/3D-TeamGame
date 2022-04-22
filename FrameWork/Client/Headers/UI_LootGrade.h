#ifndef UI_LootGrade_h__
#define UI_LootGrade_h__

#include "Hud.h"

BEGIN(Client)
class CSingleImage;
class CUI_LootGrade : public CHud
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_LootGrade(void) = default;
	explicit CUI_LootGrade(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_LootGrade(const CUI_LootGrade& rhs);
	virtual ~CUI_LootGrade() = default;

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
	void SetGrade(std::wstring _szTextureName);
	void SetFadeOut(void);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CHud::Desc    desc;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;

public:
	static CUI_LootGrade* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_LootGrade_h__

#ifndef UI_ItemStatusEffect_h__
#define UI_ItemStatusEffect_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class UI_ItemStatusEffect : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI* pOwner = nullptr;
	};

	explicit UI_ItemStatusEffect(void) = default;
	explicit UI_ItemStatusEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit UI_ItemStatusEffect(const UI_ItemStatusEffect& rhs);
	virtual ~UI_ItemStatusEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void setActive(_bool bActive) override;
public:
	HRESULT		  Ready_Component(void);
	CSingleImage* GetSingleImage(void);
	_int		  Attach_Owner(void);
	void		  SetTexture(std::wstring name);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;

private:
	_float m_fInitPos = 430.f;
	_float m_fEndPos  = 420.f;

private:
	Desc   desc;

public:
	static UI_ItemStatusEffect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_ItemStatusEffect_h__
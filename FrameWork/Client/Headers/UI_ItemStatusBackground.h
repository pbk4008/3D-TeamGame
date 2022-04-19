#ifndef UI_ItemStatusBackground_h__
#define UI_ItemStatusBackground_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class UI_ItemStatusBackground : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI* pOwner = nullptr;
	};

	explicit UI_ItemStatusBackground(void) = default;
	explicit UI_ItemStatusBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit UI_ItemStatusBackground(const UI_ItemStatusBackground& rhs);
	virtual ~UI_ItemStatusBackground() = default;

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
private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;

private:
	Desc   desc;
	
public:
	static UI_ItemStatusBackground* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_ItemStatusBackground_h__

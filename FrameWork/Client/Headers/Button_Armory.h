#ifndef Button_Armory_h__
#define Button_Armory_h__

#include "UI.h"
class CSingleImage;
BEGIN(Client)
class CSingleImage;
class CUIHelper;
class CButton_Armory final : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};
	explicit CButton_Armory(void) = default;
	explicit CButton_Armory(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CButton_Armory(const CButton_Armory& rhs);
	virtual ~CButton_Armory() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_bool   ButtonClicked(void);
	_int	Attach_Owner(void);

public:
	static CButton_Armory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUIHelper*    m_pUIHelperCom = nullptr;
	Desc desc;
	CTransform*	  m_pLocalTransform = nullptr;
	CUI*		  m_pOwner = nullptr;

private:
	virtual void Free() override;
};

END


#endif // Button_Armory_h__


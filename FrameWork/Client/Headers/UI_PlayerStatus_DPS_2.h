#ifndef UI_PlayerStatus_DPS_2_h__
#define UI_PlayerStatus_DPS_2_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_PlayerStatus_DPS_2 : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_PlayerStatus_DPS_2(void) = default;
	explicit CUI_PlayerStatus_DPS_2(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_PlayerStatus_DPS_2(const CUI_PlayerStatus_DPS_2& rhs);
	virtual ~CUI_PlayerStatus_DPS_2() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);
	void SetIcon(std::wstring szIconName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc  desc;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;

public:
	static CUI_PlayerStatus_DPS_2* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_PlayerStatus_DPS_2_h__
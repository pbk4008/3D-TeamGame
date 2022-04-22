#ifndef UI_LevelUP_Fill_Lead_Left_h__
#define UI_LevelUP_Fill_Lead_Left_h__

#include "Hud.h"

BEGIN(Client)
class CSingleImage;
class UI_LevelUP_Fill_Right;
class CUI_LevelUP_Fill_Lead_Left : public CHud
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	explicit CUI_LevelUP_Fill_Lead_Left(void) = default;
	explicit CUI_LevelUP_Fill_Lead_Left(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_LevelUP_Fill_Lead_Left(const CHud& rhs);
	virtual ~CUI_LevelUP_Fill_Lead_Left() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);

	void Set_Fill(UI_LevelUP_Fill_Right* pObj) { m_pFill = pObj; }
public:
	void SetBg(const std::wstring& _szFileName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc    desc;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	_bool		  m_bSetScale = true;

	UI_LevelUP_Fill_Right* m_pFill = nullptr;
	_float m_fRatio = 0.f;
public:
	static CUI_LevelUP_Fill_Lead_Left* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_LevelUP_Fill_Lead_Left_h__

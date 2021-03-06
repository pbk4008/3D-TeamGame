#ifndef UI_LevelUP_Fill_Left_h__
#define UI_LevelUP_Fill_Left_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CPlayerData;
class UI_LevelUP_Fill_Left : public CUI
{
public:
	struct ItemSlotDesc
	{
		_float2 fPos;
		_float2 fScale;
		CUI* pOwner = nullptr;
	};

	struct Desc
	{
		CItemData	itemData;
		_float		fDisapeatTime = 5.f;
		_int		iQueueIndex = 0;
		CUI* pOwner = nullptr;
		_float2		fInitPos = { 0.f, 0.f };
		_float2		fInitScale = { 1.2f, 1.2f };
		_float2		fEndScale = { 1.0f, 1.0f };
	};

	typedef struct RenderVal
	{
		_float		fExpRatio;//현재 경험치 비율
		_float		fGapX;
	}RENDERVAL;


	explicit UI_LevelUP_Fill_Left(void) = default;
	explicit UI_LevelUP_Fill_Left(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit UI_LevelUP_Fill_Left(const CUI& rhs);
	virtual ~UI_LevelUP_Fill_Left() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_int Attach_Owner(void);
	void SetUI(CPlayerData* pPlayerData);
	void SetFadeOut(void);
	void FadeIn(void);

public:
	void Set_ExpRatio(_float Ratio) { Bufferdesc.fExpRatio = Ratio; }

public:
	const _float Get_Ratio() const { return Bufferdesc.fGapX; }

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	Desc		  desc;
	CUI*		  m_pOwner = nullptr;
	_bool		  m_bSetScale = true;

private:
	RENDERVAL Bufferdesc;

public:
	static UI_LevelUP_Fill_Left* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#define MAX_EXP 100

#endif // UI_LevelUP_Fill_Left_h__
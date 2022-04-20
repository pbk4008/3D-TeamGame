#ifndef UI_LevelUP_Fill_Left_h__
#define UI_LevelUP_Fill_Left_h__

#include "Hud.h"

BEGIN(Client)
class CSingleImage;
class CPlayerData;
class UI_LevelUP_Fill_Left : public CHud
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
		CUI*	pOwner = nullptr;
	};

	typedef struct RenderVal
	{
		_float		 fCurExpGauge;
		_float		 fGapX;
		_float		 fGapY;
	}RENDERVAL;


	explicit UI_LevelUP_Fill_Left(void) = default;
	explicit UI_LevelUP_Fill_Left(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit UI_LevelUP_Fill_Left(const CHud& rhs);
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

public:
	void Set_ExExpRatio(_float Ratio) { m_fExExpRatio = Ratio; }
	void Set_ExpRatio(_float Ratio) { m_fExpRatio = Ratio; }

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc		  desc;
	CUI*		  m_pOwner = nullptr;
	CTransform*	  m_pLocalTransform = nullptr;
	_bool		  m_bSetScale = true;

private:
	_float m_fExExpRatio = 0.f;		//이전 경험치 비율
	_float m_fExpRatio = 0.f;		//현재 경험치 비율
	_float m_fCurExpGauge = 0.f;	//이전게이지 - 현재게이지
	_float m_fGapX = 1.f;
	_float m_fGapY = 0.5f;
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
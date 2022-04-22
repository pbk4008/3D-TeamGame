#ifndef UI_LevelUP_Fill_Right_h__
#define UI_LevelUP_Fill_Right_h__

#include "Hud.h"

BEGIN(Client)
class CSingleImage;
class UI_LevelUP_Fill_Right : public CHud
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
		_float		fExpRatio;//���� ����ġ ����
		_float		fGapX;
	}RENDERVAL;

	explicit UI_LevelUP_Fill_Right(void) = default;
	explicit UI_LevelUP_Fill_Right(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit UI_LevelUP_Fill_Right(const CHud& rhs);
	virtual ~UI_LevelUP_Fill_Right() = default;

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
	void SetBg(const std::wstring& _szFileName);

public:
	const _float Get_Ratio() const { return Bufferdesc.fGapX; }

private:
	CSingleImage*	m_pSigleImageCom = nullptr;
	Desc			desc;
	CUI*			m_pOwner = nullptr;
	CTransform*		m_pLocalTransform = nullptr;
	_bool			m_bSetScale = true;

private:
	RENDERVAL Bufferdesc;

public:
	static UI_LevelUP_Fill_Right* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_LevelUP_Fill_Right_h__
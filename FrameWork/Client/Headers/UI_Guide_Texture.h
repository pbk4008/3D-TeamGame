#ifndef UI_Guide_Texture_h__
#define UI_Guide_Texture_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_Guide_Texture  : public CUI
{
public:
	struct Desc
	{
		_float3 fPos;
		_uint	iNumTex;
		CRenderer::RENDER ERenderGroup = CRenderer::RENDER::RENDER_UI;
	};

public:
	explicit CUI_Guide_Texture(void) = default;
	explicit CUI_Guide_Texture(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Guide_Texture(const CUI_Guide_Texture& rhs);
	virtual ~CUI_Guide_Texture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	void	SetTexture(std::wstring szTexture);
	_uint	GetNumTex(void) { return desc.iNumTex; }
private:
	CSingleImage*	m_pSigleImageCom = nullptr;
	CTransform*		m_pTriggerTrans = nullptr;
	CGameObject*	m_pPlayer = nullptr;

private:
	_int m_interactDist = 5;
	Desc desc;

public:
	static CUI_Guide_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END

#endif // UI_Guide_Texture_h__
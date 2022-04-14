#ifndef UI_Background_h__
#define UI_Background_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_Background  : public CUI
{
public:
	explicit CUI_Background(void) = default;
	explicit CUI_Background(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_Background(const CUI_Background& rhs);
	virtual ~CUI_Background() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void setActive(_bool bActive) override;
public:
	HRESULT Ready_Component(void);

private:
	CSingleImage*	m_pSigleImageCom = nullptr;
	_float			m_fInitPosX = -35.f;

private:
	_float m_fInitPos = -20.f;
	_float m_fEndPos  = -30.f;

public:
	static CUI_Background* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_Background_h__


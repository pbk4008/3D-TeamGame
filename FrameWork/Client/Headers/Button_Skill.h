#ifndef Button_Skill_h__
#define Button_Skill_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUIHelper;
class CButton_Skill final : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
	};
	explicit CButton_Skill(void) = default;
	explicit CButton_Skill(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CButton_Skill(const CButton_Skill& rhs);
	virtual ~CButton_Skill() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	_bool   ButtonClicked(void);

public:
	static CButton_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	CUIHelper* m_pUIHelperCom = nullptr;
	Desc desc;

private:
	virtual void Free() override;
};

END

#endif // Button_Skill_h__

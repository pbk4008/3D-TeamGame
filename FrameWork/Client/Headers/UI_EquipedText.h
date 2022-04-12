#ifndef UI_EquipedText_h__
#define UI_EquipedText_h__

#include "UI.h"

BEGIN(Client)
class CSingleImage;
class CUI_EquipedText : public CUI
{
public:
	struct Desc
	{
		_float2 fPos;
		_float2 fScale;
	};

public:
	explicit CUI_EquipedText(void) = default;
	explicit CUI_EquipedText(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUI_EquipedText(const CUI_EquipedText& rhs);
	virtual ~CUI_EquipedText() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);

public:
	void	SetIcon(const std::wstring & _szFileName);

private:
	CSingleImage* m_pSigleImageCom = nullptr;
	Desc  desc;

public:
	static CUI_EquipedText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_EquipedText_h__


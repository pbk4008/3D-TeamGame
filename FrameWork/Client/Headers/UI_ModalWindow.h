#ifndef UI_ModalWindow_h__
#define UI_ModalWindow_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Indexes;
class CUI_Equipment;
class CUI_Armory;
class CUI_Background;
class CUI_ModalWindow : public CUI
{
public:
	explicit CUI_ModalWindow(void) = default;
	explicit CUI_ModalWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_ModalWindow(const CUI_ModalWindow& rhs);
	virtual ~CUI_ModalWindow() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void) override;
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg) override;
	virtual _int Tick(_double dDeltaTime) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void);
	HRESULT Ready_UIObject(void);

	void Show(void);
	void Hide(void);
	void ShutTheUI(void);
	_bool OpenArmory(void);
	_bool OpenEquipment(void);

public: /* Get */
	CUI_Indexes*	GetIndex(void);
	CUI_Equipment*  GeEquipment(void);
	CUI_Armory*		GetArmory(void);

public: /* Set */
	void SetIndex(CUI_Indexes* _pIndex);
	void SetEquipment(CUI_Equipment* _pEquipment);
	void SetArmory(CUI_Armory* _pArmory);

public: /* Sprites */
	CGameObject* ModalSprite = nullptr;

private:
	CUI_Indexes*    m_pIndex = nullptr;
	CUI_Equipment*  m_pEquipment = nullptr;
	CUI_Armory*	    m_pArmory = nullptr;
	CUI_Background* m_pBG = nullptr;

public:
	static CUI_ModalWindow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);

private:
	virtual void Free() override;
};

END
#endif // UI_ModalWindow_h__
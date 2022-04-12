#ifndef UIHelper_h__
#define UIHelper_h__

BEGIN(Client)

#include "Component.h"
class CUIHelper final : public CComponent
{
private:
	explicit CUIHelper(void) = default;
	explicit CUIHelper(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CUIHelper(const CUIHelper& rhs);
	virtual ~CUIHelper(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render(void);

public:
	void UpdateBoundary(CTransform* _TransForm);
	_bool IsCursorRect(void);
	POINT getMousePostion(void);
	_bool MouseClickDown(void);
	_bool MouseClickEquipBtn(void);
	_bool MouseClickArmoryBtn(void);

private:
	RECT m_Boundary;

public:
	static CUIHelper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END
#endif // UIHelper_h__

#ifndef InteractableObject_h__
#define InteractableObject_h__

#include "Actor.h"

BEGIN(Client)

class CInteractableObject abstract : public CActor
{
protected:
	explicit CInteractableObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CInteractableObject(const CInteractableObject& _rhs);
	virtual ~CInteractableObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype()								      override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime)										  override;
	virtual _int LateTick(_double _dDeltaTime)									  override;
	virtual HRESULT Render()													  override;

public:
	virtual void OnTriggerEnter(CCollision& collision)   override;
	virtual void OnTriggerExit(CCollision& collision)    override;
	virtual void OnCollisionEnter(CCollision& collision) override;
	virtual void OnCollisionStay(CCollision& collision)  override;
	virtual void OnCollisionExit(CCollision& collision)  override;

	/*for. Interaction */
public:
	virtual _bool   IsInteractable(void)	PURE;
	virtual _bool   HasInteractionUI(void)	PURE;
	virtual _float  GetRatioForUI(void)		PURE;
	virtual _vector GetGuideUIPos(void)		PURE;

public:
	virtual void BeginInteraction(void);
	virtual void UpdateInteraction(_double _dDeltaTime);
	virtual void EndInteraction(void);

public:
	_bool IsAlive() const { return !m_bDead; }
	_bool IsInteratcing(void);
	_bool IsShowGuideUI(void);
	void  ShowGuideUI(void);
	void  HideGuideUI(void);

public:
	virtual void FocusEnter(void);
	virtual void Focus(void);
	virtual void FocusExit(void);
	_bool IsFocused(void);

	/* for. Check Interacting */
public:
	_bool m_bInteracting = false;
	_bool m_bShowGuideUI = false;
	_bool m_isFocused	 = false;
	_bool m_bDead		 = false;

public:
	virtual CGameObject * Clone(const _uint _iSceneID, void* _pArg = nullptr) PURE;
	virtual void Free() override;
};

END
#endif // InteractableObject_h__

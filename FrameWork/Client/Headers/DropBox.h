#ifndef DropBox_h__
#define DropBox_h__

#include "InteractableObject.h"
#include "ItemData.h"

BEGIN(Engine)
class CAnimator;
class CModel;
class CBoxCollider;
class CLight;
END

BEGIN(Client)

class CDropBox final : public CInteractableObject
{
public:
	enum ANIM_TYPE { A_HEAD, A_IDLE, A_OPEN, A_END };

public:
	struct Desc
	{

	};

private:
	explicit CDropBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CDropBox(const CDropBox& _rhs);
	
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Set_Animation_FSM();

public:
	// CInteractableObject을(를) 통해 상속됨
	virtual _bool IsInteractable(void) override;
	virtual _bool HasInteractionUI(void) override;
	virtual _float GetRatioForUI(void) override;
	virtual _vector GetGuideUIPos(void) override;
public:
	virtual void BeginInteraction(void) override;
	virtual void UpdateInteraction(_double _dTimeDelta) override;
	virtual void EndInteraction(void) override;

public:
	virtual void FocusEnter() override;
	virtual void Focus() override;
	virtual void FocusExit() override;

public:
	_bool IsOpen(void) { return m_bBoxOpened; }

private:
	void Open(void);
	std::vector<CItemData> GetDropList(void);

private:
	CGameObject*		  m_pPlayer = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CAnimator*			  m_pAnimator = nullptr;
	CStateController*	  m_pStateController = nullptr;
	CBoxCollider*		  m_pCollider = nullptr;
	CModel*				  m_pModel = nullptr;
	MABOBJECT			  m_tDesc;
	CLight*				  m_plight = nullptr;
	_float				  m_range = 0.f;
private:
	const _float m_openDelay = 2.0f;
	_float		 m_openElapsed = 0.0f;
	_float		 m_interactDist = 5.f;
	_bool		 m_bBoxOpened = false;
	_bool		 m_bOpenAnimEnd = false;
	_bool		 m_bFocus = false;

private:
	std::vector<CItemData>m_dropList;
	_float m_dropElapsed = 0.f;
	_float m_dropDelay = 0.f;
	_float m_dropDelayMin = 0.01f;
	_float m_dropDelayMax = 0.2f;

public:
	static CDropBox* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
#endif // DropBox_h__
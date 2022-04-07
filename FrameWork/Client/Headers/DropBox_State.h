#ifndef DropBox_State_h__
#define DropBox_State_h__

#include "State.h"

BEGIN(Client)

class CDropBox_State : public CState
{
protected:
	explicit CDropBox_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CDropBox_State() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	void Set_Object(CGameObject* _pObj);
	void Set_Transform(CTransform* _pTransform);
	void Set_Model(CModel* _pModel);
	void Set_AnimationController(CAnimationController* _pAnimationController);
	void Set_Animator(CAnimator* _pAnimator);

protected:
	CGameObject*		  m_pObj = nullptr;
	CTransform*			  m_pTransform = nullptr;
	CModel*				  m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CAnimator*			  m_pAnimator = nullptr;
};

END
#endif // DropBox_State_h__


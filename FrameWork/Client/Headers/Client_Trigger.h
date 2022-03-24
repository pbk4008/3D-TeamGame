#ifndef Client_Trigger_h__
#define Client_Trigger_h__

#include "GameObject.h"

BEGIN(Engine)
class CGizmo;
class CBoxCollider;
END
BEGIN(Client)
class CClient_Trigger final : public CGameObject
{
protected:
	explicit CClient_Trigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CClient_Trigger(const CClient_Trigger& _rhs);
	virtual ~CClient_Trigger(void) = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	LateTick(_double TimeDelta) override;
	virtual HRESULT Render()					override;
private:
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual HRESULT SetUp_Components(void);
	void			Set_WVPMatrix(void);
	_fmatrix		Get_WorldMat(void);
public:
	_bool Get_OnTrigger() { return m_bOnTrigger; }
	_bool Get_Overlap() { return m_bOverlap; }
public:
	void Set_Overlap(const _bool bCheck) { m_bOverlap = bCheck; }
public:
	CGizmo*		m_pGizmo = nullptr;
	CBoxCollider* m_pBoxCollider = nullptr;//Trigger체크용
public:
	TRIGGER		m_TriggerDesc;
public:
	_bool			m_bOnTrigger;//Trigger발동됐는지 안됐는지
	_bool			m_bOverlap;//Trigger중복 방지
	_float4x4		m_matWorld;
	_float3	   m_fLocalMouse; 
	_float3	   m_vRayPos;
	_float3		m_vRayDir;
	_bool			m_bPick = false;
public:
	static  CClient_Trigger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject*  Clone(const _uint _iSceneID, void* pArg)		override;
	virtual void		  Free(void)			override;
};
END
#endif // Client_Trigger_h__

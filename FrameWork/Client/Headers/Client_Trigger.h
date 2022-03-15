#ifndef Client_Trigger_h__
#define Client_Trigger_h__

#include "GameObject.h"

BEGIN(Engine)
class CGizmo;
END

class CClient_Trigger final : public CGameObject
{
protected:
	explicit CClient_Trigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CClient_Trigger(const CClient_Trigger& _rhs);
	virtual ~CClient_Trigger(void) = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	LateTick(_double TimeDelta) override;
	virtual HRESULT Render()					override;
public:
	virtual HRESULT SetUp_Components(void);
	void			Set_WVPMatrix(void);
	_fmatrix		Get_WorldMat(void);
public:
	CGizmo*		m_pGizmo = nullptr;
public:
	TRIGGER		m_TriggerDesc;

public:
	_float4x4  m_matWorld;
	_float3	   m_fLocalMouse; 
	_float3	   m_vRayPos;
	_float3    m_vRayDir;
	_bool	   m_bPick = false;
public:
	static  CClient_Trigger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject*  Clone(void* pArg)		override;
	virtual void		  Free(void)			override;
};

#endif // Client_Trigger_h__

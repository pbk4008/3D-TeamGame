#ifndef Static_Mesh_h__
#define Static_Mesh_h__

#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

class CStatic_Mesh final : public CGameObject
{
protected:
	explicit CStatic_Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CStatic_Mesh(const CStatic_Mesh& _rhs);
	virtual ~CStatic_Mesh(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT SetUp_Components(void);
	void			Set_WVPMatrix(void);

public:
	void	Input_Key(_double _dtimeDelta);
	void    Pick_Model(void);
	void    Find_Pointer(void);
	_bool   IntersectTriangle(const _float3& orig, const _float3& dir, _float3& v0, _float3& v1, _float3& v2, _float* t, _float* u, _float* v);


public:
	CModel*		m_pModelCom = nullptr;
	MODELDESC*	m_ModelDesc;

public:
	_float4x4  m_matWorld;
	_float3	   m_fLocalMouse; 
	POINT	   m_tMousePos;
	_float3	   m_vRayPos;
	_float3    m_vRayDir;
	_bool	   m_bPick = false;

public:
	static CStatic_Mesh* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free(void) override;
};

#endif // Static_Mesh_h__
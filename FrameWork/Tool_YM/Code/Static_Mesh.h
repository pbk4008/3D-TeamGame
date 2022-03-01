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
	void	Input_Key(_double _dtimeDelta);
	void    Pick_Model(void);
	void    Find_Pointer(void);
	_bool   IntersectTriangle(const _fvector& orig, const _fvector& dir, _fvector& v0, _fvector& v1, _fvector& v2, _float* t, _float* u, _float* v);
public:
	CModel*		m_pModelCom = nullptr;
	MODELDESC	m_ModelDesc;
	MESHDESC	m_MeshDesc;
	POINT	    m_tMousePos;
public:
	_float4x4  m_matWorld;
	_float3	   m_fLocalMouse; 
	_float3	   m_vRayPos;
	_float3    m_vRayDir;
	_bool	   m_bPick = false;
public:
	static  CStatic_Mesh* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject*  Clone(void* pArg)		override;
	virtual void		  Free(void)			override;
};

#endif // Static_Mesh_h__
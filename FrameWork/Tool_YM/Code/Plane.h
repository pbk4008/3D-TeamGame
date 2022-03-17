#ifndef Plane_h__
#define Plane_h__

#include "Tool_Defines.h"
#include "GameObject.h"
#include "VIBuffer_Plane.h"
#include "Transform.h"
#include "Cell.h"

BEGIN(Engine)
class CVIBuffer_Plane;
class CRenderer;
class CTexture;
class CNavigation;
END

class CPlane final : public CGameObject
{
protected:
	explicit CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CPlane(const CPlane& _rhs);
	virtual ~CPlane(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	LateTick(_double TimeDelta)override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT SetUp_Components();
	HRESULT			Create_NavigationLine();
	HRESULT			Make_NavigationCell(class CCell* _pCell = nullptr);
	HRESULT			Update_CellPos();
	void			CCW_Sort(_float3* pPoints[]);
	void			Input_Key(_double _dtimeDelta);

public:
	void    Pick_Model(void);
	void    Find_Pointer(void);
	_bool   IntersectTriangle(const _fvector& orig, const _fvector& dir, _float3& v0, _float3& v1, _float3& v2, _float* t, _float* u, _float* v);

public:
	CVIBuffer_Plane* m_pVIBufferCom = nullptr;
	class CNavigation* m_pNavigationCom = nullptr;
	CTexture* m_pTexture = nullptr;

public:
	void			Set_WVPMatrix();
	void*			Get_Indices(void) { return m_pVIBufferCom->Get_Indices(); }
	void*			Get_Vertices(void) { return m_pVIBufferCom->getVertices(); }
	_uint			Get_NumFaces(void) { return m_pVIBufferCom->Get_NumFaces(); }
	_uint			Get_VerticesX(void) { return m_pVIBufferCom->getVerteicesX(); }
	_uint			Get_VerticesZ(void) { return m_pVIBufferCom->getVerteicesZ(); }
	_fmatrix		Get_WorldMatrix(void) { return m_pTransform->Get_WorldMatrix(); }

public:
	_bool		m_bOneClick = true;
	_bool		m_bRender = true;
	_float3*	m_fPoints[3];
	_float3     m_fPosition = { 5.f, 5.f, 5.f };
	_float3		m_fLocalMouse;
	_int		m_iPointindex = 0;

public:
	_float4x4  m_matWorld;
	_float3	   m_vRayPos;
	_float3    m_vRayDir;
	_bool	   m_bPick = false;
	POINT	   m_tMousePos;
public:
	class CObserver* m_pObserver = nullptr;

public:
	static CPlane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg) override;
	virtual void Free(void) override;
};


#endif // Plane_h__
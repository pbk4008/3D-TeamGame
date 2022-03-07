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
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	LateTick(_double TimeDelta)override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT SetUp_Components();
	HRESULT			Create_NavigationLine();
	HRESULT			Make_NavigationCell(class CCell* _pCell = nullptr);
	HRESULT			Update_CellPos();
	void			CCW_Sort(_float3* pPoints);
	void			Input_Key(_double _dtimeDelta);

public:
	CVIBuffer_Plane* m_pVIBufferCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;
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
	class CObserver* m_pObserver = nullptr;

public:
	static CPlane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free(void) override;
};


#endif // Plane_h__
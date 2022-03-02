#ifndef NavSphere_h__
#define NavSphere_h__

#include "GameObject.h"

BEGIN(Engine)
class CModel;
END
class CNavSphere final : public CGameObject
{
protected:
	explicit CNavSphere(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CNavSphere(const CNavSphere& _rhs);
	virtual ~CNavSphere(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Input_Key(_double _dtimeDelta);
	void    Pick_Model(void);
	void    Find_Pointer(void);
	_bool   IntersectTriangle(const _fvector& orig, const _fvector& dir, _fvector& v0, _fvector& v1, _fvector& v2, _float* t, _float* u, _float* v);
public:
	virtual HRESULT SetUp_Components(void);
	void			Set_WVPMatrix(void);

public:
	CModel* m_pModelCom = nullptr;
	POINT	    m_tMousePos;
public:
	_float3    m_fPostion;
	_float3	   m_fLocalMouse;
	_float3    m_fSpherePosition;
	_float3	   m_vRayPos;
	_float3    m_vRayDir;
	_bool	   m_bPick = false;

public:
	// CGameObject을(를) 통해 상속됨
	static CNavSphere* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual Engine::CGameObject* Clone(void* pArg);
	virtual void Free(void) override;
};

#endif // NavSphere_h__

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
	CModel*		m_pModelCom = nullptr;
	MODELDESC*	m_ModelDesc;

public:
	static CStatic_Mesh* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free(void) override;
};

#endif // Static_Mesh_h__
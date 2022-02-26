#ifndef Mesh_Save_h__
#define Mesh_Save_h__

#include "Base.h"
#include "Tool_Defines.h"

class CMesh_Save : public CBase
{
protected:
	CMesh_Save(void);
	virtual ~CMesh_Save(void) = default;

public:
	HRESULT NativeConstruct(const MODELDESC& _ModelDesc, CGameObject* _pGameObject);
	HRESULT Get_ModelDesc(CGameObject* _pGameObject);

public:
	MODELDESC m_ModelDesc;

public:
	_float4x4 m_WorldMat;

public:
	static CMesh_Save* Create(const MODELDESC& _ModelDesc, CGameObject* _pGameObject);
	void Free(void) override;
}

#endif // Mesh_Save_h__
#include "pch.h"
#include "Mesh_Save.h"
#include "Static_Mesh.h"

CMesh_Save::CMesh_Save(void)
{
}

HRESULT CMesh_Save::NativeConstruct(const MODELDESC& _ModelDesc, CGameObject* _pGameObject)
{
	memcpy(&m_ModelDesc, &_ModelDesc, sizeof(MODELDESC));

	if (FAILED(Get_ModelDesc(_pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh_Save::Get_ModelDesc(CGameObject* _pGameObject)
{
	CStatic_Mesh* pMesh = dynamic_cast<CStatic_Mesh*>(_pGameObject);

	if (nullptr == pMesh)
		return E_FAIL;

	return S_OK;
}

CMesh_Save* CMesh_Save::Create(const MODELDESC& _ModelDesc, CGameObject* _pGameObject)
{
	CMesh_Save* pInstance = new CMesh_Save();

	if (FAILED(pInstance->NativeConstruct(_ModelDesc, _pGameObject)))
	{
		MSGBOX("Faild to Create CSaveModel!!!");
		return nullptr;
	}
	return pInstance;
}

void CMesh_Save::Free(void)
{
}

#include "pch.h"
#include "ModelObject.h"
#include "Model.h"
#include "MeshContainer.h"

CModelObject::CModelObject()
	: m_pModel(nullptr)
	, m_tModelName(L"")
	, m_tModelFolder(L"")
	, m_bDraw(false)
{
}

CModelObject::CModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pModel(nullptr)
	, m_tModelName(L"")
	, m_tModelFolder(L"")
	, m_bDraw(false)
{

}

CModelObject::CModelObject(const CModelObject& rhs)
	: CGameObject(rhs)
	, m_pModel(rhs.m_pModel)
	, m_tModelName(rhs.m_tModelName)
	, m_tModelFolder(rhs.m_tModelFolder)
	, m_bDraw(rhs.m_bDraw)
	, m_vecPick(rhs.m_vecPick)
{
	Safe_AddRef(m_pModel);
}

HRESULT CModelObject::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModelObject::NativeConstruct(void* pArg)
{
	MODELOBJDESC tDesc = (*(MODELOBJDESC*)pArg);

	m_tModelName = tDesc.wstrFbxName;
	m_tModelFolder = tDesc.wstrFbxFolder;

	if (FAILED(SetUp_Components(0, m_tModelName, L"Com_Model", (CComponent * *)& m_pModel)))
		return E_FAIL;

	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	m_vecPick.resize(m_pModel->Get_NumMeshContainer());

	return S_OK;
}

_int CModelObject::Tick(_double dDeltaTime)
{
	m_bDraw = false;



	return _int();
}

_int CModelObject::LateTick(_double dDeltaTime)
{
	if (!m_pRenderer)
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	return _int();
}

HRESULT CModelObject::Render()
{
	if (!m_bDraw)
		return S_OK;

	_matrix matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	_uint iMeshCnt = m_pModel->Get_MaterialCount();

	for (_uint i = 0; i < iMeshCnt; i++)
	{
		if (m_vecPick[i])
			m_pModel->Render(i, 1);
		else
			m_pModel->Render(i, 0);
	}

	return S_OK;
}

_bool CModelObject::Check_Name(const wstring& pName)
{
	if (pName == m_tModelName)
		return true;
	return false;
}

HRESULT CModelObject::Check_Texture(list<wstring>& pTextrue)
{
	_finddata_t fd;
	ZeroMemory(&fd, sizeof(_finddata_t));

	string strTexturePath;
	strTexturePath.assign(m_tModelFolder.begin(), m_tModelFolder.end());
	strTexturePath += "*.tga";

	intptr_t handle = _findfirst(strTexturePath.c_str(), &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;
	while (iResult != -1)
	{
		string strName = fd.name;
		wstring wstrTextureTag;
		wstring wstrTexturePath;
		wstrTexturePath.assign(strTexturePath.begin(), strTexturePath.end());
		wstrTextureTag.assign(strName.begin(), strName.end());
		if (!g_pGameInstance->Get_Texture(wstrTextureTag))
			g_pGameInstance->Add_Texture(m_pDevice, wstrTextureTag, wstrTexturePath);

		pTextrue.emplace_back(wstrTextureTag);
		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	return S_OK;
}

void CModelObject::Focusing()
{
	_vector vCamPos = g_pGameInstance->Get_CamPosition(L"MainCamera");
	_matrix matView = g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	_matrix matCamWorld=XMMatrixInverse(nullptr, matView);
	_vector vCamLook=matCamWorld.r[2];
	vCamLook = XMVector3Normalize(vCamLook);

	vCamPos += vCamLook * 10.f;
	
	m_pTransform->Set_State(CTransform::STATE_POSITION, vCamPos);
}

void CModelObject::Picking_Face(_fvector vRayPos, _fvector vRayDir)
{
	_matrix matWorldInv = m_pTransform->Get_WorldMatrixInverse();
	_vector vTmpRayPos, vTmpRayDir;


	vTmpRayPos = XMVectorSetW(vRayPos, 1.f);
	vTmpRayPos=XMVector3TransformCoord(vTmpRayPos, matWorldInv);

	vTmpRayDir=XMVector3TransformNormal(vRayDir, matWorldInv);
	vTmpRayDir = XMVector3Normalize(vTmpRayDir);

	vector<vector<CMeshContainer*>> vecMeshContainer = m_pModel->Get_MeshContainer();
	_uint iCurFaceIndex = 0;

	for (auto& bPick : m_vecPick)
		bPick = false;

	for (auto& pMtrlContainer : vecMeshContainer)
	{
		for (auto& pMesh : pMtrlContainer)
		{
			void* pVertices = pMesh->getVertices();
			
			FACEINDICES32* Indices = (FACEINDICES32*)pMesh->getIndices();
			_uint iNumFaces = pMesh->Get_NumFaces();

			for (_uint i = 0; i < iNumFaces; ++i)
			{
				_float3 v0 = ((VTXMESH*)pVertices)[Indices[i]._0].vPosition;
				_float3 v1 = ((VTXMESH*)pVertices)[Indices[i]._1].vPosition;
				_float3 v2 = ((VTXMESH*)pVertices)[Indices[i]._2].vPosition;

				_vector Pos_1 = XMLoadFloat3(&v0);
				_vector Pos_2 = XMLoadFloat3(&v1);
				_vector Pos_3 = XMLoadFloat3(&v2);

				Pos_1 = XMVectorSetW(Pos_1, 1.f);
				Pos_2 = XMVectorSetW(Pos_2, 1.f);
				Pos_3 = XMVectorSetW(Pos_3, 1.f);

				_float fDist = 0.f;
						// Check if the pick ray passes through this point
				_float fBary1, fBary2;
				if (TriangleTests::Intersects(vTmpRayPos, vTmpRayDir, Pos_1, Pos_2, Pos_3, fDist))
				{
					m_vecPick[iCurFaceIndex] = true;
					return;
				}
			}
			iCurFaceIndex++;
		}
	}
}

CModelObject* CModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CModelObject* pInstance = new CModelObject(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CModelObject Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CModelObject::Clone(void* pArg)
{
	CModelObject* pInstance = new CModelObject(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CModelObject Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CModelObject::Free()
{
	CGameObject::Free();
	Safe_Release(m_pModel);
}

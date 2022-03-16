#include "StaticObject.h"
#include "GameInstance.h"
#include <tchar.h>

CStaticObject::CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext)
	: CGameObject(pDevice,pDevcieContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
	: CGameObject(rhs)
{
}

void CStaticObject::Set_WVPMatrix()
{
	_matrix matWorldInvers = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
}

void CStaticObject::Set_LightMatrix()
{
	_matrix matLightView = XMMatrixTranspose(m_Lightdesc->mLightView);
	_matrix matLightProj = XMMatrixTranspose(m_Lightdesc->mLightProj);

	m_pModel->SetUp_ValueOnShader("g_LightView", &matLightView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_LightProj", &matLightProj, sizeof(_matrix));

}

HRESULT CStaticObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_ModelDesc = *(MODELDESC*)pArg;

	if (m_ModelDesc.WorldMat._42 > -100.f)
		m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_ModelDesc.WorldMat));

	_float4x4 world;
	XMStoreFloat4x4(&world, m_pTransform->Get_WorldMatrix());
	world._41 = m_ModelDesc.fInitPos.x;
	world._42 = m_ModelDesc.fInitPos.y;
	world._43 = m_ModelDesc.fInitPos.z;

	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&world));
	m_pTransform->Set_TransformDesc(10.0f, XMConvertToRadians(45.f));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//m_MeshDesc.iType = m_ModelDesc.iType;
	//m_MeshDesc.eLevel = m_ModelDesc.eLevel;
	//_tcscpy_s(m_MeshDesc.FolderName, m_ModelDesc.strFolder.c_str());
	//_tcscpy_s(m_MeshDesc.FileName, m_ModelDesc.strFileName.c_str());
	//_tcscpy_s(m_MeshDesc.Name, m_ModelDesc.strName.c_str());
	//_tcscpy_s(m_MeshDesc.Tag, m_ModelDesc.strTag.c_str());


	return S_OK;
}

_int CStaticObject::Tick(_double TimeDelta)
{

	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_bNomralMapping = true;
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		m_bNomralMapping = false;

	return _int();
}

_int CStaticObject::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer&& g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		if (m_pRenderer->Get_Shadow())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

		if (m_pRenderer->Get_PBR())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PBR, this);

		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	}

	return _int();
}

HRESULT CStaticObject::Render()
{
	Set_WVPMatrix();
	m_pModel->SetUp_ValueOnShader("g_bool", &m_bNomralMapping, sizeof(_bool));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE);
		m_pModel->SetUp_TextureOnShader("g_BiNormalTexture", i, aiTextureType_NORMALS);
		m_pModel->Render(i, 2);
	}

	return S_OK;
}

HRESULT CStaticObject::Render_Shadow()
{
	Set_WVPMatrix();
	Set_LightMatrix();

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE);
		m_pModel->Render(i, 3);
	}

	return S_OK;
}

HRESULT CStaticObject::Render_ShadeShadow(ID3D11ShaderResourceView* SRV)
{
	Set_WVPMatrix();
	Set_LightMatrix();

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE);
		m_pModel->SetUp_TextureOnShader("g_ShadowTexture", SRV);
		m_pModel->Render(i, 4);
	}

	return S_OK;
}

HRESULT CStaticObject::Render_PBR()
{
	_bool nontex = true;

	Set_WVPMatrix();

	m_pModel->SetUp_ValueOnShader("g_bool", &m_bNomralMapping, sizeof(_bool));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 5);
	

	return S_OK;
}

HRESULT CStaticObject::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, m_ModelDesc.strFileName, L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_Lightdesc = g_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

CStaticObject* CStaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStaticObject* pInstance = new CStaticObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CStaticObject* CStaticObject::Clone(void* pArg)
{
	CStaticObject* pInstance = new CStaticObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free(void)
{
	__super::Free();

	Safe_Release(m_pModel);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

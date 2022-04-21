#include "pch.h"
#include "TestObj.h"
#include "GameInstance.h"
#include "Material.h"

CTestObj::CTestObj()
{
}

CTestObj::CTestObj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CTestObj::CTestObj(const CTestObj& rhs)
	: CGameObject(rhs)
{

}

HRESULT CTestObj::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Test", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Cinema/Test_box/test_box.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Test", pMtrl);

	return S_OK;
}

HRESULT CTestObj::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Test", L"Test", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Test"), 0);


	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);

	_vector pos = { 0,2,0,1 };
	m_pTransform->Set_State(CTransform::STATE_POSITION, pos);

	return S_OK;
}

_int CTestObj::Tick(_double TimeDelta)
{
	_vector pos = { -175.f, 54.f, 422.f,1 };
	m_pTransform->Set_State(CTransform::STATE_POSITION, pos);

	return _int();
}

_int CTestObj::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CTestObj::Render()
{
	wstring CameraTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix matworld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix ViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(CameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix ProjMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(CameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matworld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));

	for (_uint i = 0; i < m_pModel->Get_MaterialCount(); i++)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModel->Render(i, 2);
	}

	return S_OK;
}

void CTestObj::OnCollisionEnter(CCollision& _collision)
{
	if (typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::OnCollisionExit(CCollision& _collision)
{
	if (typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::OnTriggerEnter(CCollision& _collision)
{
	if (typeid(*_collision.pGameObject) == typeid(CTestObj))
	{

	}
}

void CTestObj::setIsMove(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CTestObj::move(_double dDeltaTime)
{
	if (!m_isMove)
		return;

	if (g_pGameInstance->getkeyPress(DIK_RIGHT))
		m_pTransform->Go_Right(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_UP))
		m_pTransform->Go_Straight(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_LEFT))
		m_pTransform->Go_Left(dDeltaTime);
	if (g_pGameInstance->getkeyPress(DIK_DOWN))
		m_pTransform->Go_BackWard(dDeltaTime);
}

CTestObj* CTestObj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestObj* pInstance = new CTestObj(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTestObj Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTestObj::Clone(const _uint _iSceneID, void* pArg)
{
	CTestObj* pInstance = new CTestObj(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CTestObj Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObj::Free()
{
	CGameObject::Free();

	Safe_Release(m_pModel);
}

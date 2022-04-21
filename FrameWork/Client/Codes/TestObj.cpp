#include "pch.h"
#include "TestObj.h"

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

	_matrix matPivot = XMMatrixIdentity();
	m_pModel = CModel::Create(m_pDevice, m_pDeviceContext, "../bin/FBX/Cinema/Test_box/", "test_po.fbx"
		, L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC,true);
	m_pModel = static_cast<CModel*>(m_pModel->Clone(nullptr));

	return S_OK;
}

HRESULT CTestObj::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	
	return S_OK;
}

_int CTestObj::Tick(_double TimeDelta)
{

	return _int();
}

_int CTestObj::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CTestObj::Render()
{
	_matrix matIdentity = XMMatrixIdentity();
	wstring CameraTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix ViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(CameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix ProjMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(CameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matIdentity, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &ProjMatrix, sizeof(_matrix));

	_uint iSize = m_pModel->Get_MaterialCount();
	for (_uint i = 0; i < iSize; i++)
		m_pModel->Render(i, 2);

	return S_OK;
}

void CTestObj::OnCollisionEnter(CCollision& _collision)
{
	if(typeid(*_collision.pGameObject) == typeid(CTestObj))
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
}

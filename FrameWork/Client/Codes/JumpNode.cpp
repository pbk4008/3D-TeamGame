#include "pch.h"
#include "JumpNode.h"

CJumpNode::CJumpNode(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CJumpNode::CJumpNode(const CJumpNode& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CJumpNode::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CJumpNode::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CJumpNode::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = m_pAnimationController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CJumpNode::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

HRESULT CJumpNode::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CJumpNode::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(10.f, 4.f, 10.f, 1.f));

	if (FAILED(SetUp_Components(m_iSceneID, L"Model_JumpNode", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_pModel->Set_PivotMatrix(matPivot);

	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Idle_JumpNode");


	CPhysicsXSystem::COLDESC tColDesc;
	tColDesc.bGravity = false;
	tColDesc.bKinematic = false;
	tColDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_STATIC;
	XMStoreFloat3(&tColDesc.fPos, m_pTransform->Get_State(CTransform::STATE_POSITION));

	CBoxCollider::BOXDESC tBoxColDesc;
	tBoxColDesc.tColDesc = tColDesc;
	XMStoreFloat4x4(&tBoxColDesc.matTransform, m_pTransform->Get_CombinedMatrix());
	tBoxColDesc.pParent = this;
	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColDesc)))
		return E_FAIL;

	return S_OK;
}

CJumpNode* CJumpNode::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CJumpNode* pInstance = new CJumpNode(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CJumpNode Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CJumpNode::Clone(const _uint _iSceneID, void* _pArg)
{
	CJumpNode* pInstance = new CJumpNode(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CJumpNode Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJumpNode::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}

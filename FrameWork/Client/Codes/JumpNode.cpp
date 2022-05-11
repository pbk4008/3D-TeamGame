#include "pch.h"
#include "JumpNode.h"

#include "Material.h"

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

	m_iObectTag = (_uint)GAMEOBJECT::JUMP_NODE;

	//CMaterial* pMtrl = nullptr;
	//CTexture* pTexture = nullptr;
	//// 0 밖에 놈
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_JumpNode", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/T_jumpNode_D.dds", 1);
	//pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/T_jumpNode_N.dds", 1);
	//pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/T_jumpNode_ORME.dds", 1);
	//pMtrl->Set_Texture("g_ORMETexture", TEXTURETYPE::TEX_ORME, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_JumpNode", pMtrl);
	//// 1 내부 원
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_JumpPoint", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/towerALL_07_-_Default_BaseColor.dds", 1);
	//pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/towerALL_07_-_Default_Normal.dds", 1);
	//pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/JumpNode/towerALL_07_-_Default_OcclusionRoughnessMetallic.dds", 1);
	//pMtrl->Set_Texture("g_ORMTexture", TEXTURETYPE::TEX_ORM, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_JumpPoint", pMtrl);

	return S_OK;
}

HRESULT CJumpNode::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CJumpNode::Tick(_double _dDeltaTime)
{
	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 50.f))
		return 0;

	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = m_pAnimationController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_isChange)
	{
		switch (m_isPick)
		{
		case false:
			if (m_pAnimationController->Is_Finished())
			{
				m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Idle_JumpNode");
				m_isChange = false;
			}
			break;
		case true:
			if (m_pAnimationController->Is_Finished())
			{
				m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Activated_Shake_JumpNode");
				m_pAnimationController->Set_PlaySpeed(0.6f);
				m_isChange = false;
			}
			break;
		}
	}

	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CJumpNode::LateTick(_double _dDeltaTime)
{
	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 6.f))
		return 0;

	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;
	return _int();
}

HRESULT CJumpNode::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{

		if (i == 1)
		{
			_float4 color = _float4(0.3725f, 0.2941f, 0.5450f, 1.f);
			_float EmissionPower = 0.f;
			if(m_isPick)
				EmissionPower = 1.5f;
			else
				EmissionPower = 0.3f;
			if (FAILED(m_pModel->SetUp_ValueOnShader("g_color", &color, sizeof(_float4))))		return E_FAIL;
			if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &EmissionPower, sizeof(_float))))		return E_FAIL;
		}

		m_pModel->Render(i, i);
	}

#ifdef _DEBUG

#endif // _DEBUG


	return S_OK;
}

HRESULT CJumpNode::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_tDesc.vPosition), 1.f));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_JumpNode_Bin", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.008f, 0.008f, 0.008f);
	m_pModel->Set_PivotMatrix(matPivot);
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_JumpNode"), 0);
	//m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_JumpPoint"), 1);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Idle_JumpNode");

	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.isSceneQuery = true;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 3.f, 4.f, 2.4f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, tBoxColliderDesc.vScale.y * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::JumpTrigger);

	return S_OK;
}

void CJumpNode::setIsPick(const _bool _isPick)
{
	if (m_isPick != _isPick)
	{
		switch (_isPick)
		{
		case true:
			m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Activating_JumpNode", false);
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Jump_Node);
			g_pGameInstance->Play_Shot(L"JumpNode_Open", CSoundMgr::CHANNELID::Jump_Node);
			break;
		case false:
			m_pAnimationController->SetUp_NextAnimation("SK_Jump_Node.ao|A_Deactivating_JumpNode", false);
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Jump_Node);
			g_pGameInstance->Play_Shot(L"JumpNode_Close", CSoundMgr::CHANNELID::Jump_Node);
			break;
		}
		m_pAnimationController->Set_PlaySpeed(1.f);
		m_isChange = true;
		m_isPick = _isPick;
	}
}

void CJumpNode::OnTriggerEnter(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();

	if ((_uint)SCENEID::SCENE_STATIC == iTag)
	{
		int a = 0;
	}
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

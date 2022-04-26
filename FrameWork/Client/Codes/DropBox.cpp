#include "pch.h"
#include "DropBox.h"

#include "DropManager.h"
#include "Light.h"
#include "DropBox_State.h"
#include "DropBox_Idle.h"
#include "DropBox_Open.h"

CDropBox::CDropBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CInteractableObject(_pDevice, _pDeviceContext)
	, m_pAnimationController(nullptr)
	, m_pAnimator(nullptr)
	, m_pStateController(nullptr)
	, m_openElapsed(0.0f)
	, m_interactDist(5.f)
	, m_bBoxOpened(false)
	, m_bOpenAnimEnd(false)
	, m_bFocus(false)
{
}

CDropBox::CDropBox(const CDropBox& _rhs)
	: CInteractableObject(_rhs)
	, m_openElapsed(_rhs.m_openElapsed)
	, m_interactDist(_rhs.m_interactDist)
	, m_bBoxOpened(_rhs.m_bBoxOpened)
	, m_bOpenAnimEnd(_rhs.m_bOpenAnimEnd)
	, m_bFocus(_rhs.m_bFocus)
{
}

HRESULT CDropBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropBox::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Set_Animation_FSM()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_tDesc = (*(DROPBOXDESC*)_pArg);
	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_tDesc.WorldMat));

	m_pPlayer = m_pPlayer = *g_pGameInstance->getObjectList(_iSceneID, L"Layer_Silvermane")->begin();
	assert(m_pPlayer);

	m_iObectTag = (_uint)GAMEOBJECT::DROP_BOX;

	setActive(true);

	//박스위치에 파티클생성
	_matrix matPos = m_pTransform->Get_WorldMatrix();
	matPos.r[3] = XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) - 0.5f);
	Active_Effect_Target((_uint)EFFECT::BOX , matPos);

	//light 
	LIGHTDESC LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 7.f;
	LightDesc.vDiffuse = _float4(0.3686f, 04941.f, 0.60784f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.bactive = true;
	LightDesc.vPosition = _float3(m_tDesc.WorldMat._41, m_tDesc.WorldMat._42 + 2.f, m_tDesc.WorldMat._43);

	m_range = LightDesc.fRange;

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc,&m_plight))) MSGBOX("Failed To Adding PointLight");

	return S_OK;
}

_int CDropBox::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pCollider->Tick(_dDeltaTime);

	/* State FSM Update */
	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_plight->Set_Color(XMVectorSet(0.2f, 1.f, 0.5f, 1.f));

	if (true == m_bBoxOpened && 0 < m_dropList.size())
	{
		m_dropElapsed += (_float)_dDeltaTime; /* drop time */
		CLEAR_QUEST(L"T_HUD_Find_DropBox");
		if (m_dropElapsed >= m_dropDelay) 
		{
			g_pGameInstance->StopSound(CSoundMgr::CHANNELID::Item_Drop);
			g_pGameInstance->Play_Shot(L"Drop_Item_2", CSoundMgr::CHANNELID::Item_Drop);
			g_pGameInstance->VolumeChange(CSoundMgr::CHANNELID::Item_Drop, 5.5f);
			m_dropElapsed = 0.f;
			m_pCollider->Remove_ActorFromScene();
			m_dropDelay = (_float)MathUtils::ReliableRandom(m_dropDelayMin, m_dropDelayMax);
			CDropManager::GetInstance()->DropItem(m_dropList.back(), m_pTransform->Get_State(CTransform::STATE_POSITION), EScatterType::Cone, Get_Transform());
			m_dropList.pop_back();
		}
	}

	if (m_bBoxOpened == true)
	{
		m_range += (g_fDeltaTime * -2.f);
		m_plight->Set_Range(m_range);

		if (m_range <= 0.f)
			m_plight->Set_Active(false);
	}

	return _int();
}

_int CDropBox::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	/* State FSM Late Update */
	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CDropBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CDropBox::Ready_Components()
{
	/* for. Transform Com */
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_DropBox", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	/* for. Collider Com */
	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.isSceneQuery = true;
	tColliderDesc.isTrigger = false;
	tColliderDesc.pGameObject = this;
	tColliderDesc.isGravity = false;

	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 2.0f, 1.f, 1.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, tBoxColliderDesc.vScale.y * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::ItemBox);

	/* for. Animator Com */
	matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pModel->Set_PivotMatrix(matPivot);

	CAnimator::ANIMATORDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.pModel = m_pModel;
	tDesc.pTransform = m_pTransform;
	tDesc.eType = CAnimationController::EType::CharacterController;

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", L"Com_Animator", (CComponent**)&m_pAnimator, &tDesc)))
		return E_FAIL;

	/* for. State Controller */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	/* for. Anim Controller */
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->Set_MoveSpeed(1.f);

	return S_OK;
}

HRESULT CDropBox::Ready_States()
{
	if(FAILED(m_pStateController->Add_State(L"DropBox_Open", CDropBox_Open::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CDropBox_State*>(pair.second)->Set_Object(this);
		static_cast<CDropBox_State*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CDropBox_State*>(pair.second)->Set_Model(m_pModel);
		static_cast<CDropBox_State*>(pair.second)->Set_AnimationController(m_pAnimationController);
		static_cast<CDropBox_State*>(pair.second)->Set_Animator(m_pAnimator);
	}
	m_pStateController->Change_State(L"DropBox_Open");

	return S_OK;
}

HRESULT CDropBox::Set_Animation_FSM()
{
	CAnimation* pAnimation =  m_pModel->Get_Animation("Take 001");
	if (FAILED(m_pAnimator->Insert_Animation((_uint)ANIM_TYPE::A_OPEN, (_uint)ANIM_TYPE::A_HEAD, pAnimation, FALSE, FALSE, FALSE, ERootOption::XYZ)))
		return E_FAIL;

	m_pAnimator->Insert_AnyEntryAnimation((_uint)ANIM_TYPE::A_OPEN);

	return S_OK;
}

void CDropBox::Open(void)
{
	m_dropList = GetDropList();
}

std::vector<CItemData> CDropBox::GetDropList(void)
{
	return m_tDesc.itemData;
}

_bool CDropBox::IsInteractable(void)
{
	if (IsFocused() && m_bFocus && false == m_bBoxOpened)
	{
		_float dist = MathUtils::Length(m_pPlayer, this);
		if (m_interactDist >= dist)
		{
			/* Guide UI Show */
			return true;
		}
	}
	return false;
}

_bool CDropBox::HasInteractionUI(void)
{
	return _bool();
}

_float CDropBox::GetRatioForUI(void)
{
	return _float();
}

_vector CDropBox::GetGuideUIPos(void)
{
	return _vector();
}

void CDropBox::BeginInteraction(void)
{
	__super::BeginInteraction();
	m_openElapsed = 0.f;
}

void CDropBox::UpdateInteraction(_double _dTimeDelta)
{
	__super::UpdateInteraction(_dTimeDelta);

	m_pStateController->Change_State(L"DropBox_Open");
	m_bBoxOpened = true;
	Open();
}

void CDropBox::EndInteraction(void)
{
	__super::EndInteraction();

	m_openElapsed = 0.f;
	HideGuideUI();
}

void CDropBox::FocusEnter()
{
	__super::FocusEnter();
}

void CDropBox::Focus()
{
	__super::Focus();
	Active_Effect((_uint)EFFECT::OPENBOX);

	m_bFocus = true;
}

void CDropBox::FocusExit()
{
	__super::FocusExit();

	m_bFocus = false;
}

CDropBox* CDropBox::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CDropBox* pInstance = new CDropBox(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CDropBox Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDropBox::Clone(const _uint _iSceneID, void* _pArg)
{
	CDropBox* pInstance = new CDropBox(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CDropBox Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDropBox::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationController);
	Safe_Release(m_pStateController);
	Safe_Release(m_pCollider);
	Safe_Release(m_pAnimator);
}
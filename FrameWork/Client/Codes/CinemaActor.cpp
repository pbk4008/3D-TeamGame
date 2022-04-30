#include "pch.h"
#include"CinemaActor.h"
#include "HierarchyNode.h"


CCinemaActor::CCinemaActor()
	: m_pController(nullptr)
	, m_iActorTag(0)
{
}

CCinemaActor::CCinemaActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CActor(pDevice, pDeviceContext)
	, m_pController(nullptr)
	, m_iActorTag(0)
{
}

CCinemaActor::CCinemaActor(const CCinemaActor& rhs)
	: CActor(rhs)
	, m_pController(rhs.m_pController)
	, m_iActorTag(0)
{
}

HRESULT CCinemaActor::NativeConstruct_Prototype()
{
	if (FAILED(CActor::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCinemaActor::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(CActor::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;
	
	_uint iActorTag = (*(_uint*)pArg);

	if (FAILED(Ready_Model(iActorTag)))
		return E_FAIL;


	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimController",(CComponent**)&m_pController)))
		return E_FAIL;

	m_pController->Set_Model(m_pModel);
	m_pController->Set_Transform(m_pTransform);
	m_pController->Set_MoveSpeed(40.f);

	return S_OK;
}

_int CCinemaActor::Tick(_double TimeDelta)
{
	//m_pTransform->Set_WorldMatrix(XMMatrixIdentity());
	//m_pModel->Update_CombinedTransformationMatrix(TimeDelta);
	if(!m_pController->Is_Finished())
		m_pController->Tick(TimeDelta);

	return _int();
}

_int CCinemaActor::LateTick(_double TimeDelta)
{
	if(m_pRenderer != nullptr)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA,this);

	return _int();
}

HRESULT CCinemaActor::Render()
{
	if(FAILED(Render_Acoter()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCinemaActor::Ready_Model(_uint iModel)
{
	HRESULT hr = S_OK;
	switch (iModel)
	{
	case (_uint)CINEMA_ACTOR::ACTOR_SILVERMANE:
		hr=CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Cinema_Silvermane", L"Model", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_ACTOR::ACTOR_GRAYEHAWK:
		hr=CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_GrayHawk", L"Model", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_ACTOR::ACTOR_PHOENIX:
		hr=CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Phoenix", L"Model", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS:
		hr=CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_MidBoss", L"Model", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_ACTOR::ACTOR_SCREE:
		hr=CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Scree", L"Model", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_ACTOR::ACTOR_BOSS:
		//hr=CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Scree", L"Model", (CComponent**)&m_pModel);
		break;
	}

	if (FAILED(hr))
		MSGBOX("Model SetUp Fail");

	m_iActorTag = iModel;

	return S_OK;
}

HRESULT CCinemaActor::Render_Acoter()
{
	wstring CameraTag = g_pGameInstance->Get_BaseCameraTag();
	if (m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

		CActor::BindConstantBuffer(CameraTag);
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			SCB desc;
			ZeroMemory(&desc, sizeof(SCB));
			switch (i)
			{
			case 0: case 3:  // body
				desc.metalic = 0.5f;
				desc.roughness = 0.f;
				desc.ao = 0.5f;
				desc.color = _float4(0.811f, 1.f, 0.898f, 1.f);
				desc.empower = 0.7f;
				CActor::BindConstantBuffer(wstrCamTag, &desc);
				m_pModel->Render(i, 0);
				break;
			case 1: // fur
				desc.color = _float4(1.f, 0.466f, 0.901f, 1.f);
				desc.empower = 0.01f;
				CActor::BindConstantBuffer(wstrCamTag, &desc);
				m_pModel->Render(i, 1);
				break;
			case 2:  // cloak
				CActor::BindConstantBuffer(wstrCamTag, &desc);
				m_pModel->Render(i, 2);
				break;
			}
		}
	}
	else if(m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_GRAYEHAWK)
	{
		_float4 color = _float4(1.f, 1.f, 1.f, 1.f);
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_MainColor", &color, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			SCB desc;
			ZeroMemory(&desc, sizeof(SCB));

			switch (i)
			{
			case 0:
				CActor::BindConstantBuffer(CameraTag, &desc);
				if (FAILED(m_pModel->Render(i, 1)))	MSGBOX("Failed To Rendering Actor");
				break;
			default:
				desc.metalic = 0.3f;
				desc.roughness = 0.1f;
				desc.ao = 0.4f;
				desc.color = _float4(0.f,0.f,1.f,1.f);
				desc.empower = 1.f;
				CActor::BindConstantBuffer(CameraTag,&desc);
				if (FAILED(m_pModel->Render(i, 0)))	MSGBOX("Failed To Rendering Actor");
				break;
			}
		}

	}
	else if (m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_PHOENIX)
	{
		_float4 color = _float4(1.f, 1.f, 1.f, 1.f);
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_MainColor", &color, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

		SCB desc;
		ZeroMemory(&desc, sizeof(SCB));
		desc.metalic = 0.5f;
		desc.roughness = 0.3f;
		desc.ao = 0.8f;
		CActor::BindConstantBuffer(CameraTag,&desc);
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pModel->Render(i, 1)))	MSGBOX("Failed To Rendering Actor");
		}
	}
	else
	{
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); i++)
		{
			SCB desc;
			ZeroMemory(&desc, sizeof(SCB));

			if (i == 0)
			{
				desc.color = _float4(0.498f, 0.9411f, 0.8196f, 1.f);;
				desc.empower = 0.5f;
				CActor::BindConstantBuffer(CameraTag, &desc);
			}
			else
				CActor::BindConstantBuffer(CameraTag, &desc);

			if (FAILED(m_pModel->Render(i, i))) MSGBOX("Fialed To Rendering Silvermane");
		}
	}
	return S_OK;
}

void CCinemaActor::Actor_AnimPlay(_int iIndex)
{
	m_pController->Set_Animation(iIndex);
}

void CCinemaActor::AnimSpeed(_float fSpeed)
{
	m_pController->Set_PlaySpeed(fSpeed);
}

void CCinemaActor::Actor_AnimReset()
{
	if(m_pController)
		m_pController->Reset_Animation();
}

void CCinemaActor::Acotr_AnimFrameSet(_double iFrame)
{
	if (m_pController)
		m_pController->Add_TrackAcc(iFrame);
}

_uint CCinemaActor::Get_AnimFrame()
{
	if (!m_pController)
		return 0;

	return m_pController->Get_CurAnimIndex();
}

CHierarchyNode* CCinemaActor::Get_Bone(const string& tBoneName)
{
	CHierarchyNode* pBone=m_pModel->Get_Bone(tBoneName);

	return pBone;
}

_fmatrix CCinemaActor::Get_Pivot()
{
	return m_pModel->Get_PivotMatrix();
}

CCinemaActor* CCinemaActor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCinemaActor* pInstance = new CCinemaActor(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCineamActor Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCinemaActor::Clone(const _uint iSceneID, void* pArg)
{
	CCinemaActor* pInstance = new CCinemaActor(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CCineamActor Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCinemaActor::Free()
{
	CActor::Free();
	Safe_Release(m_pController);
}

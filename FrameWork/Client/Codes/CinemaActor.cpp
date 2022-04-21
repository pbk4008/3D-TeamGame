#include "pch.h"
#include"CinemaActor.h"

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
	Safe_AddRef(m_pController);
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
	}

	if (FAILED(hr))
		MSGBOX("Model SetUp Fail");

	m_iActorTag = iModel;

	return S_OK;
}

HRESULT CCinemaActor::Render_Acoter()
{
	wstring CameraTag = g_pGameInstance->Get_BaseCameraTag();
	CActor::BindConstantBuffer(CameraTag);
	if (m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS)
	{
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pModel->Render(i, i)))
				MSGBOX("Fialed To Rendering Silvermane");
		}
	}
	else if(m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_GRAYEHAWK)
	{
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			switch (i)
			{
			case 0:
				if (FAILED(m_pModel->Render(i, 2)))
					MSGBOX("Failed To Rendering Actor");
				break;
			case 1:
				if (FAILED(m_pModel->Render(i, 1)))
					MSGBOX("Failed To Rendering Actor");
				break;
			case 2:
				if (FAILED(m_pModel->Render(i, 0)))
					MSGBOX("Failed To Rendering Actor");
				break;
			}
		}
	}
	else if (m_iActorTag == (_uint)CINEMA_ACTOR::ACTOR_PHOENIX)
	{
		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			switch (i)
			{
			case 0:
				if (FAILED(m_pModel->Render(i, 1)))
					MSGBOX("Failed To Rendering Actor");
				break;
			case 1:
				if (FAILED(m_pModel->Render(i, 0)))
					MSGBOX("Failed To Rendering Actor");
				break;
			case 2:
				if (FAILED(m_pModel->Render(i, 2)))
					MSGBOX("Failed To Rendering Actor");
				break;
			case 3:
				if (FAILED(m_pModel->Render(i, 3)))
					MSGBOX("Failed To Rendering Actor");
			}
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

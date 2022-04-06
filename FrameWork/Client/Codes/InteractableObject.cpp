#include "pch.h"
#include "InteractableObject.h"
#include "InteractManager.h"

CInteractableObject::CInteractableObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CInteractableObject::CInteractableObject(const CInteractableObject& _rhs)
	: CActor(_rhs)
	, m_bInteracting(_rhs.m_bInteracting)
	, m_bShowGuideUI(_rhs.m_bShowGuideUI)
{
	CInteractManager::GetInstance()->AddInteractObject(this);
}

HRESULT CInteractableObject::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInteractableObject::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;
	return S_OK;
}

_int CInteractableObject::Tick(_double _dDeltaTime)
{
	if (FAILED(__super::Tick(_dDeltaTime)))
		return -1;

	return _int();
}

_int CInteractableObject::LateTick(_double _dDeltaTime)
{
	if (FAILED(__super::LateTick(_dDeltaTime)))
		return -1;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this))) 
		return -1;


	return _int();
}

HRESULT CInteractableObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	/*if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, i)))
			return E_FAIL;
	}*/

	return S_OK;
}


void CInteractableObject::BeginInteraction(void)
{
	assert(false == m_bInteracting);
	m_bInteracting = true;
}

void CInteractableObject::UpdateInteraction(_double _dDeltaTime)
{
}

void CInteractableObject::EndInteraction(void)
{
	assert(true == m_bInteracting);
	m_bInteracting = false;
}

_bool CInteractableObject::IsInteratcing(void)
{
	return m_bInteracting;
}

_bool CInteractableObject::IsShowGuideUI(void)
{
	return m_bShowGuideUI;
}

void CInteractableObject::ShowGuideUI(void)
{
	m_bShowGuideUI = true;
}

void CInteractableObject::HideGuideUI(void)
{
	m_bShowGuideUI = false;
}

void CInteractableObject::FocusEnter(void)
{
	m_isFocused = true;
}

void CInteractableObject::Focus(void)
{
}

void CInteractableObject::FocusExit(void)
{
	m_isFocused = false;
}

_bool CInteractableObject::IsFocused(void)
{
	return m_isFocused;
}

void CInteractableObject::OnTriggerEnter(CCollision& collision)
{
	__super::OnTriggerEnter(collision);
}

void CInteractableObject::OnTriggerExit(CCollision& collision)
{
	__super::OnTriggerExit(collision);
}

void CInteractableObject::OnCollisionEnter(CCollision& collision)
{
	__super::OnCollisionEnter(collision);
}

void CInteractableObject::OnCollisionStay(CCollision& collision)
{
	__super::OnCollisionStay(collision);
}

void CInteractableObject::OnCollisionExit(CCollision& collision)
{
	__super::OnCollisionExit(collision);
}

void CInteractableObject::Free()
{
	__super::Free();
}



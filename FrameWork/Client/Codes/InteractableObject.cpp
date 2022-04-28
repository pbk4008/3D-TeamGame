#include "pch.h"
#include "InteractableObject.h"
#include "InteractManager.h"
#include "Effect.h"

CInteractableObject::CInteractableObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CGameObject(_pDevice, _pDeviceContext)
{
}

CInteractableObject::CInteractableObject(const CInteractableObject& _rhs)
	: CGameObject(_rhs)
	, m_bInteracting(_rhs.m_bInteracting)
	, m_bShowGuideUI(_rhs.m_bShowGuideUI)
	, m_bInteractDead(_rhs.m_bInteractDead)
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

void CInteractableObject::Active_Effect(_uint iEffectIndex)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		_vector Mypos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		Mypos = XMVectorSetY(Mypos, XMVectorGetY(Mypos) + 1.f);
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
}

void CInteractableObject::Active_Effect(_uint iEffectIndex, _fvector vPivot)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		_vector Mypos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		Mypos += vPivot;
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
}

void CInteractableObject::Active_Effect_Target(_uint iEffectIndex, _matrix TargetMat)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		TargetMat.r[3] = XMVectorSetY(TargetMat.r[3], XMVectorGetY(TargetMat.r[3]) + 1.f);
		pEffect->Get_Transform()->Set_WorldMatrix(TargetMat);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
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
	Safe_Release(m_pModel);
}



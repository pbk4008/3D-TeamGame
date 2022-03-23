#include "pch.h"
#include "JumpTrigger.h"

CJumpTrigger::CJumpTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CJumpTrigger::CJumpTrigger(const CJumpTrigger& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CJumpTrigger::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CJumpTrigger::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CJumpTrigger::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//Raycast_FromMouse(_dDeltaTime);

	m_pCollider->Update(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CJumpTrigger::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

HRESULT CJumpTrigger::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pCollider->Render(L"Camera_Silvermane");
#endif // _DEBUG

	return S_OK;
}

HRESULT CJumpTrigger::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(25.f, 2.f, 84.f, 1.f));


	CRay_Collider::COLLIDERDESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(CRay_Collider::COLLIDERDESC));
	tColliderDesc.vScale = { 4.f, 2.f, 2.f };
	tColliderDesc.vPosition = { 0.f, 2.f, 0.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_RayCollider", L"Collider", (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CJumpTrigger::Raycast_FromMouse(const _double& _dDeltaTime)
{
	_matrix smatView;

	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);

	_vector svRayPos, svRayDir;
	memcpy_s(&svRayPos, sizeof(_vector), &smatView.r[3], sizeof(_vector));
	memcpy_s(&svRayDir, sizeof(_vector), &smatView.r[2], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);

	_float fOutDist = 0.f;
	if (m_pCollider->Raycast_AABB(svRayPos, svRayDir, fOutDist))
	{
		m_fHoldTime += (_float)_dDeltaTime;
		m_isPick = true;
	}
	else
	{
		m_fHoldTime = 0.f;
		m_isPick = false;
	}
}

_bool CJumpTrigger::Raycast(const _fvector& _svRayPos, const _fvector& _svRayDir, _float& _fOutDist, const _double & _dDeltaTime)
{
	if(m_pCollider->Raycast_AABB(_svRayPos, _svRayDir, _fOutDist))
	{
		m_fHoldTime += (_float)_dDeltaTime;
		m_isPick = true;
	}
	else
	{
		m_fHoldTime = 0.f;
		m_isPick = false;
	}

	return m_isPick;
}

CJumpTrigger* CJumpTrigger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CJumpTrigger* pInstance = new CJumpTrigger(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CJumpTrigger Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CJumpTrigger::Clone(const _uint _iSceneID, void* _pArg)
{
	CJumpTrigger* pInstance = new CJumpTrigger(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CJumpTrigger Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJumpTrigger::Free()
{
	Safe_Release(m_pCollider);

	__super::Free();
}

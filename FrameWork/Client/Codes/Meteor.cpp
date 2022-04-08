#include "pch.h"
#include "Meteor.h"

CMeteor::CMeteor()
	: m_pModel(nullptr)
	, m_pCollider(nullptr)
	, m_fSpeed(0.f)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
}

CMeteor::CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pModel(nullptr)
	, m_pCollider(nullptr)
	, m_fSpeed(0.f)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
}

CMeteor::CMeteor(const CMeteor& rhs)
	: CGameObject(rhs)
	, m_pModel(rhs.m_pModel)
	, m_pCollider(rhs.m_pCollider)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDestination(rhs.m_vDestination)
{
	Safe_AddRef(m_pModel);
	Safe_AddRef(m_pCollider);
}

HRESULT CMeteor::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_METEOR;

	return S_OK;
}

HRESULT CMeteor::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
	{
		_vector vPos = (*(_vector*)_pArg);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransform->Scaling(XMVectorSet(0.1f, 0.1f, 0.1f,0.f));
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fSpeed = 100.f;

	XMStoreFloat4(&m_vDestination, g_pObserver->Get_PlayerPos());

	Move();

	return S_OK;
}

_int CMeteor::Tick(_double _dDeltaTime)
{
	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CMeteor::LateTick(_double _dDeltaTime)
{
	m_pCollider->Update_Transform();

	if (!m_pRenderer)
		return E_FAIL;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CMeteor::Render()
{
	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		if (FAILED(m_pModel->Render(i, 0))) 	return E_FAIL;

	return S_OK;
}

HRESULT CMeteor::Ready_Component()
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = 0.f;

	m_pTransform->Set_TransformDesc(tDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Shooter_Bullet", L"BulletModel", (CComponent**)&m_pModel)))
		return E_FAIL;

	CSphereCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 0.5f;
	tColliderDesc.tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.tColliderDesc.isTrigger = true;
	tColliderDesc.tColliderDesc.isGravity = true;
	tColliderDesc.tColliderDesc.fMass = 10.f;
	tColliderDesc.tColliderDesc.isVisualization = true;
	tColliderDesc.tColliderDesc.isSceneQuery = true;
	tColliderDesc.tColliderDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CMeteor::OnTriggerEnter(CCollision& collision)
{
	m_bRemove = true;
}

_int CMeteor::Move()
{	
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vDest = XMLoadFloat4(&m_vDestination);
	_vector vDir = vDest-vPos;
	vDir=XMVectorSetY(vDir, 0.f);
	_float fLen = XMVectorGetX(XMVector3Length(vDir));

	vDir = XMVector3Normalize(vDir);
	fLen /=8;
	vDir *= fLen*m_fSpeed;
	vDir=XMVectorSetY(vDir, 30.f);
	m_pCollider->Add_Force(vDir);

	return _int();
}



CMeteor* CMeteor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMeteor* pInstance = new CMeteor(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMeteor Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeteor::Clone(const _uint iSceneID, void* pArg)
{
	CMeteor* pInstance = new CMeteor(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CMeteor Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor::Free()
{
	CGameObject::Free();
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

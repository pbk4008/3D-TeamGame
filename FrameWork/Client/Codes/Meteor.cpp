#include "pch.h"
#include "Meteor.h"
#include "Animation.h"

CMeteor::CMeteor()
	: m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_pCollider(nullptr)
	, m_fSpeed(0.f)
	, m_bRemoveCheck(false)
	, m_fRandSpawnTime(0.f)
	, m_fAccTime(0.f)
	, m_bStart(false)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
}

CMeteor::CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	,m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_pCollider(nullptr)
	, m_fSpeed(0.f)
	, m_bRemoveCheck(false)
	, m_fRandSpawnTime(0.f)
	, m_fAccTime(0.f)
	, m_bStart(false)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
}

CMeteor::CMeteor(const CMeteor& rhs)
	: CGameObject(rhs)
	, m_pStaticModel(rhs.m_pStaticModel)
	, m_pAnimModel(rhs.m_pAnimModel)
	, m_pCollider(rhs.m_pCollider)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDestination(rhs.m_vDestination)
	, m_bRemoveCheck(rhs.m_bRemoveCheck)
	, m_fRandSpawnTime(rhs.m_fRandSpawnTime)
	, m_fAccTime(rhs.m_fAccTime)
	, m_bStart(false)
{
	Safe_AddRef(m_pStaticModel);
	Safe_AddRef(m_pAnimModel);
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

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimModel->SetUp_AnimationIndex(0);
	m_fSpeed = 1000.f;


	return S_OK;
}

_int CMeteor::Tick(_double _dDeltaTime)
{
	if (!m_bStart)
	{
		m_fAccTime += (_float)_dDeltaTime;
		if (m_fAccTime >= m_fRandSpawnTime)
		{
			m_bStart = true;
			m_fAccTime = 0.f;
			m_fRandSpawnTime = 0.f;
		}
		else
			return 0;
	}
	m_pCollider->Tick(_dDeltaTime);

	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float fY = XMVectorGetY(vPos);

	if (fY < -30.f)
		m_bRemove = true;
	
	if (m_bRemoveCheck)
	{
		m_pTransform->Scaling(XMVectorSet(0.3f, 0.3f, 0.3f, 0.f));
		m_pAnimModel->Update_CombinedTransformationMatrix(_dDeltaTime*3.f);
		vector<CAnimation*> pVecAnim = m_pAnimModel->Get_Animations();

		if (pVecAnim[0]->Is_Finished())
		{
			pVecAnim[0]->Reset_Animation();
			setActive(false);
			m_bStart = false;
			//m_bRemove = true;
		}
	}

	return _int();
}

_int CMeteor::LateTick(_double _dDeltaTime)
{
	if (!m_bStart)
		return 0; 

	if (!m_bRemoveCheck)
		m_pCollider->Update_Transform();

	if (!m_pRenderer)
		return E_FAIL;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CMeteor::Render()
{
	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (!m_bRemoveCheck)
	{
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
			return E_FAIL;
		for (_uint i = 0; i < m_pStaticModel->Get_NumMeshContainer(); ++i)
			if (FAILED(m_pStaticModel->Render(i, 0))) 	return E_FAIL;
	}
	else
	{
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pAnimModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
			return E_FAIL;
		for (_uint i = 0; i < m_pAnimModel->Get_NumMeshContainer(); ++i)
			m_pAnimModel->Render(i, 0);
	}
	

	return S_OK;
}

HRESULT CMeteor::Ready_Component()
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = 0.f;

	m_pTransform->Set_TransformDesc(tDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Healer_Meteor_Static", L"Meteor_Static", (CComponent**)&m_pStaticModel)))
		return E_FAIL;
	_matrix matIdnetity = XMMatrixIdentity();
	m_pStaticModel->Set_PivotMatrix(matIdnetity);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Healer_Meteor_Anim", L"Meteor_Anim", (CComponent**)&m_pAnimModel)))
		return E_FAIL;
	//_matrix matScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//m_pAnimModel->Set_PivotMatrix(matScale);
	

	CSphereCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 1.5f;
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
	if (!m_bRemoveCheck)
	{
		m_bRemoveCheck = true;
		OVERLAPDESC tOverlapDesc;
		tOverlapDesc.geometry = PxSphereGeometry(5.f);
		XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
		CGameObject* pHitObject = nullptr;
		tOverlapDesc.ppOutHitObject = &pHitObject;
		tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
		if (g_pGameInstance->Overlap(tOverlapDesc))
		{
			if (tOverlapDesc.vecHitObjects.empty()
				|| !Find_HitPlayer(&tOverlapDesc.vecHitObjects))
				return;

			_uint iSize = (_uint)tOverlapDesc.vecHitObjects.size();
			for (_uint i = 0; i < iSize; ++i)
			{
				CActor* pActor = static_cast<CActor*>(tOverlapDesc.vecHitObjects[i]);
				_uint iTag = tOverlapDesc.vecHitObjects[i]->getTag();
				if (iTag == (_uint)GAMEOBJECT::PLAYER)
				{
					ATTACKDESC tAttackDesc;

					tAttackDesc.pOwner = this;
					tAttackDesc.pHitObject = pActor;

					tAttackDesc.iLevel = 3;
					tAttackDesc.fDamage = 15.f;

					pActor->Hit(tAttackDesc);
					return;
				}
				else
					continue;
			}
		}
	}
}
_int CMeteor::Move(_fvector vPos)
{	
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	XMStoreFloat4(&m_vDestination, g_pObserver->Get_PlayerPos());

	_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vDest = XMLoadFloat4(&m_vDestination);

	_vector vTmp = XMVectorSetY(vPosition, XMVectorGetY(vDest));
	_vector vDir = vDest - vTmp;
	vDir = XMVector3Normalize(vDir);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector vTmpLook = XMVector3Cross(vDir, vUp);
	_matrix matRot=XMMatrixRotationAxis(vTmpLook, XMConvertToRadians(60.f));
	
	vDir=XMVector3TransformNormal(vDir, matRot);
	//_float fLen = XMVectorGetX(XMVector3Length(vDir));

	vDir *= m_fSpeed;
	m_pCollider->Add_Force(vDir);

	m_fRandSpawnTime = (_float)MathUtils::ReliableRandom(1.f, 3.f);

	return _int();
}

_bool CMeteor::Find_HitPlayer(vector<CGameObject*>* pVecActor)
{
	for (auto& pActor : *pVecActor)
	{
		if (pActor->getTag() == (_uint)GAMEOBJECT::PLAYER)
			return true;
	}
	return false;
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
	Safe_Release(m_pStaticModel);
	Safe_Release(m_pAnimModel);
	Safe_Release(m_pCollider);
}

#include "pch.h"
#include "..\Headers\Bullet.h"
#include "SphereCollider.h"

CBullet::CBullet(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CGameObject(_pDevice, _pDeviceContext)
	, m_pModelCom(nullptr)
	, m_pCollider(nullptr)
	, m_fSpawnTime(0.f)
	, m_fSpeed(0.f)
{
	ZeroMemory(&m_fDir, sizeof(_float4));
	ZeroMemory(&m_matBulletPosMatrix, sizeof(_float4x4));
}

CBullet::CBullet(const CBullet& _rhs)
	: CGameObject(_rhs)
	, m_pModelCom(_rhs.m_pModelCom)
	, m_pCollider(_rhs.m_pCollider)
	, m_fSpawnTime(0.f)
	, m_fDir(_rhs.m_fDir)
	, m_fSpeed(_rhs.m_fSpeed)
	, m_matBulletPosMatrix(_rhs.m_matBulletPosMatrix)
{
	Safe_AddRef(m_pModelCom);
	Safe_AddRef(m_pCollider);

}

HRESULT CBullet::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_BULLET;
	m_fSpeed = 50.f;
	return S_OK;
}

HRESULT CBullet::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
	{
		_matrix matMatrix = (*(_matrix*)_pArg);
		XMStoreFloat4x4(&m_matBulletPosMatrix, matMatrix);
		if (FAILED(Set_Spawn()))
			return E_FAIL;
	}

	if (FAILED(Ready_Component(_iSceneID)))
		return E_FAIL;

	m_fDamage = 3.f;

	return S_OK;
}

_int CBullet::Tick(_double _dDeltaTime)
{
	_uint iProgress = Move(_dDeltaTime);
	if (iProgress == OBJ_DEAD)
		return 0;

	m_pCollider->Tick(_dDeltaTime);

	if ((_uint)GAMEOBJECT::PLAYER == m_pOwner->getTag() && !m_bRemove)
		g_pObserver->Set_IsThrownObject(true);

	return _int();
}

_int CBullet::LateTick(_double _dDeltaTime)
{
	if (!m_pRenderer)
		return E_FAIL;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return _int();
}

HRESULT CBullet::Render()
{
	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;



	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
		if (FAILED(m_pModelCom->Render(i, 0))) 	return E_FAIL;

	return S_OK;
}

HRESULT CBullet::Ready_Component(const _uint iSceneID)
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = m_fSpeed;

	m_pTransform->Set_TransformDesc(tDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Shooter_Bullet", L"BulletModel", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CSphereCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 0.5f;
	tColliderDesc.tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.tColliderDesc.isTrigger = true;
	tColliderDesc.tColliderDesc.isVisualization = true;
	tColliderDesc.tColliderDesc.isSceneQuery = true;
	tColliderDesc.tColliderDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider,&tColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::Set_Spawn()
{
	_matrix matMatrix = XMLoadFloat4x4(&m_matBulletPosMatrix);
	_matrix matRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	
	_vector vLook = matMatrix.r[2];
	vLook = XMVector3TransformNormal(vLook, matRotate);
	matMatrix.r[2] = vLook;

	_matrix matTranslation = XMMatrixTranslation(0.f, 1.3f, 0.f);
	matMatrix *= matTranslation;

	m_pTransform->Set_WorldMatrix(matMatrix);

	return S_OK;
}

CActor* CBullet::Get_Owner() const
{
	return m_pOwner;
}

void CBullet::Set_Owner(CActor* _pOwner)
{
	m_pOwner = _pOwner;

	if ((_uint)GAMEOBJECT::PLAYER == _pOwner->getTag())
		m_pCollider->setShapeLayer((_uint)ELayer::Weapon);
	else
		m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);
}

void CBullet::OnTriggerEnter(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();
	if (iTag == m_pOwner->getTag())
		return;

	if ((_uint)GAMEOBJECT::PLAYER == iTag ||
		(_uint)GAMEOBJECT::MONSTER_SHOOTER == iTag)
	{
		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		//tAttackDesc.iLevel = 3;
		tAttackDesc.pHitObject = this;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);

		setActive(false);
		m_bRemove = true;
		g_pObserver->Set_IsThrownObject(false);
	}
}

_uint CBullet::Move(_double dDeltaTime)
{
	m_fSpawnTime += (_float)dDeltaTime;
	if (m_fSpawnTime < 5.f)
	{
		m_pTransform->Go_Straight(dDeltaTime);
		//_vector vDir = XMLoadFloat4(&m_fDir);

		//_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

		//vPos += vDir*dDeltaTime*m_fSpeed;

		//m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
	{
		m_fSpawnTime = 0.f;
		setActive(false);
		m_bRemove = true;
		return OBJ_DEAD;
	}
	return _uint();
}

CBullet* CBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBullet* pInstance = new CBullet(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBullet Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet::Clone(const _uint iSceneID, void* pArg)
{
	CBullet* pInstance = new CBullet(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CBullet Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pCollider);
}

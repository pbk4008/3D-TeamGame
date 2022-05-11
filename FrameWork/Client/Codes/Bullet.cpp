#include "pch.h"
#include "..\Headers\Bullet.h"
#include "SphereCollider.h"
#include "Material.h"
#include "Texture.h"

CBullet::CBullet(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CMeshEffect(_pDevice, _pDeviceContext)
	, m_pCollider(nullptr)
	, m_fSpawnTime(0.f)
	, m_fSpeed(0.f)
{
	ZeroMemory(&m_fDir, sizeof(_float4));
	ZeroMemory(&m_matBulletPosMatrix, sizeof(_float4x4));
}

CBullet::CBullet(const CBullet& _rhs)
	: CMeshEffect(_rhs)
	, m_fSpawnTime(0.f)
	, m_fDir(_rhs.m_fDir)
	, m_fSpeed(_rhs.m_fSpeed)
	, m_matBulletPosMatrix(_rhs.m_matBulletPosMatrix)
{
}

HRESULT CBullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_BULLET;
	m_fSpeed = 30.f;

	m_pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_MeshEffect", L"../bin/ShaderFile/Shader_MeshEffect.hlsl", CMaterial::EType::Static);

	return S_OK;
}

HRESULT CBullet::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
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

	m_fDamage = 13.f;

	m_fAlpha = 1.f;
	m_fFlowSpeedAlpha = 1.f;

	return S_OK;
}

_int CBullet::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	///////////////////////////////////// UV
	m_vTiling.x = 1.f;
	m_vTiling.y = 6.f;
	// X
	m_isFlowX = true;
	m_fFlowSpeedX = 0.2f;
	m_vPlusUV.x += m_fFlowSpeedX * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.x)
		m_vPlusUV.x = 0.f;
	// Y
	m_isFlowY = true;
	m_fFlowSpeedY = 0.2f;
	m_vPlusUV.y += m_fFlowSpeedY * (_float)_dDeltaTime;
	if (1.f < m_vPlusUV.y)
		m_vPlusUV.y = 0.f;

	///////////////////////////////// Color
	m_isCustomColor = true;
	m_vColor = { 0.156f, 0.36f, 1.f };


	//////////////////////////////////////////// Scale
	m_vScale = { 2.f, 2.f, 2.f };
	m_pTransform->Scaling(_vector{ m_vScale.x, m_vScale.y, m_vScale.z, 0.f });

	//////////////////////////////////////////// Rotation
	//m_pTransform->SetUp_Rotation(_float3(90.f, 0.f, 0.f));

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
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	if (m_fAccTime > m_fLifeTime)
	{
		m_fAccTime = 0.f;
	}
	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CBullet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float weight = 1.f;
	m_pModel->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));

	_float empower = 0.35f;
	m_pModel->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 7)))
			return E_FAIL;
	}

	/*_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))	return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))	return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))	return E_FAIL;

	_float3 color = _float3(1, 1, 1.f);
	_float empower = 1.f;
	_float weight = 1.f;
	_float alpha = 1.f;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_vColor", &color, sizeof(_float3)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_fAlpha", &alpha, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 7))) return E_FAIL;
	}*/

	return S_OK;
}

HRESULT CBullet::Ready_Component(const _uint iSceneID)
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = m_fSpeed;

	m_pTransform->Set_TransformDesc(tDesc);

	CSphereCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 0.5f;
	tColliderDesc.tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.tColliderDesc.isTrigger = true;
	tColliderDesc.tColliderDesc.isVisualization = true;
	tColliderDesc.tColliderDesc.isSceneQuery = true;
	tColliderDesc.tColliderDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider,&tColliderDesc)))
		return E_FAIL;

	//
	if (FAILED(m_pTexture->Change_Texture(L"Venus_Trail")))
		return E_FAIL;

	if (FAILED(m_pMaterial->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, m_pTexture, 0)))
		return E_FAIL;
	Safe_AddRef(m_pTexture);

	m_pModel = g_pGameInstance->Clone_Component<CModel>((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere2");
	if (FAILED(m_pModel->Add_Material(m_pMaterial, 0)))
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

	Safe_Release(m_pCollider);
}

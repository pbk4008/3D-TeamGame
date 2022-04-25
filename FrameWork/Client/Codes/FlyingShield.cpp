#include "pch.h"
#include "FlyingShield.h"
#include "Material.h"

#include "HierarchyNode.h"
#include "SplineCurve.h"

CFlyingShield::CFlyingShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CFlyingShield::CFlyingShield(const CFlyingShield& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CFlyingShield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_FlyingShield", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_FlyingShield", pMtrl);

	return S_OK;
}

HRESULT CFlyingShield::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
	{
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fDamage = 3.f;
	m_fSpeed = 30.f;

	m_pSpline = new CSplineCurve();

	setActive(false);
	return S_OK;
}

_int CFlyingShield::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fAccTime += (_float)_dDeltaTime;
	if (!m_isReturn)
	{
		if (m_fAccTime >= m_fLiveTime)
		{
			Return();
		}
		else
		{
			_vector svPoint = m_pSpline->GetPoint(m_fAccTime / m_fLiveTime);
			svPoint = XMVectorSetW(svPoint, 1.f);
			m_pTransform->Set_State(CTransform::STATE_POSITION, svPoint);
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), _dDeltaTime);
		}
	}
	else
	{
		if (m_fAccTime >= m_fLiveTime * 0.8f)
		{
			_vector svOwnerPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			_vector svDir = XMVector3Normalize(svOwnerPos - svPos);

			svPos += svDir * m_fSpeed * (_float)_dDeltaTime;
			m_pTransform->Set_State(CTransform::STATE_POSITION, svPos);
		}
		else
		{
			_vector svPoint = m_pSpline->GetPoint(m_fAccTime / m_fLiveTime);
			svPoint = XMVectorSetW(svPoint, 1.f);
			m_pTransform->Set_State(CTransform::STATE_POSITION, svPoint);
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), -_dDeltaTime);
		}
	}

	m_pCollider->Tick(_dDeltaTime);

	m_fMTTime += g_fDeltaTime;
	if (0.05f < m_fMTTime)
	{
		static_cast<CSilvermane*>(m_pOwner)->Create_MotionTrail(m_motiontrailidx,false, true);
		++m_motiontrailidx;
		m_fMTTime = 0.f;
	}

	if (m_motiontrailidx >= 20)
		m_motiontrailidx = 0;

	return _int();
}

_int CFlyingShield::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFlyingShield::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.color = _float4(0.f, 0.2862f, 0.5490f, 1.f);
	desc.empower = 0.8f;

	CWeapon::BindConstantBuffer(L"Camera_Silvermane", &desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CFlyingShield::Render_Shadow()
{
	CWeapon::BindConstantBuffer(L"Camera_Silvermane");
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CFlyingShield::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	switch (bActive)
	{
	case true:
		m_pCollider->Add_ActorToScene();
		break;
	case false:
		m_isReturn = false;
		m_fAccTime = 0.f;
		m_pCollider->Remove_ActorFromScene();
		break;
	}
}

void CFlyingShield::OnTriggerEnter(CCollision& collision)
{
	if (m_isReturn)
	{
		if (collision.pGameObject == m_pOwner)
		{
			setActive(false);
			static_cast<CSilvermane*>(m_pOwner)->End_ThrowShield();
		}
		return;
	}

	_uint iTag = collision.pGameObject->getTag();
	switch (iTag)
	{
	case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
	case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
	case (_uint)GAMEOBJECT::MONSTER_1H:
	case (_uint)GAMEOBJECT::MONSTER_2H:
	case (_uint)GAMEOBJECT::MONSTER_HEALER:
	case (_uint)GAMEOBJECT::MONSTER_SHOOTER:
	case (_uint)GAMEOBJECT::MONSTER_SPEAR:
	case (_uint)GAMEOBJECT::MONSTER_ANIMUS:
	case (_uint)GAMEOBJECT::MIDDLE_BOSS:
	case (_uint)GAMEOBJECT::BOSS:
		if (!m_isAttack)
			return;

		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		tAttackDesc.pHitObject = this;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);

		g_pObserver->Set_IsThrownObject(false);
		m_isAttack = false;

		Return();
		break;
	}
}

void CFlyingShield::Throw(const _fvector& _svTargetPos)
{
	setActive(true);
	XMStoreFloat3(&m_tDesc.vTargetPos, _svTargetPos);
	m_pTransform->Set_WorldMatrix(m_tDesc.pOriginTransform->Get_WorldMatrix());

	// 날아가야할 방향벡터 구하는 곳
	_vector svDir = XMVectorSetW(_svTargetPos, 1.f) - m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_fDis = XMVectorGetX(XMVector3Length(svDir));
	svDir = XMVector3Normalize(svDir);
	XMStoreFloat3(&m_vDir, svDir);

	// 방향벡터를 기준으로 라업룩 회전시켜주는곳
	_vector svLook = XMVector3Normalize(svDir) * m_pTransform->Get_Scale(CTransform::STATE_LOOK);
	_vector svRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), svDir) * m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
	_vector svUp = XMVector3Cross(svDir, XMVector3Normalize(svRight)) * m_pTransform->Get_Scale(CTransform::STATE_UP);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, svRight);
	m_pTransform->Set_State(CTransform::STATE_UP, svUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, svLook);

	m_pTransform->Rotation_Axis(CTransform::STATE_LOOK, XMConvertToRadians(45.f));
	m_pTransform->Rotation_Axis(CTransform::STATE_RIGHT, XMConvertToRadians(-15.f));


	Spline_Throw();


	m_fLiveTime = m_fDis / m_fSpeed;
	m_isAttack = true;
	g_pObserver->Set_IsThrownObject(true);
}

HRESULT CFlyingShield::Ready_Components()
{
	m_pTransform->Set_TransformDesc(0.f, 1.f);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_FlyingShield", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_FlyingShield"), 0);

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 1.f, 0.2f, 1.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	m_pCollider->setShapeLayer((_uint)ELayer::Weapon);

	return S_OK;
}

void CFlyingShield::Return()
{
	m_isReturn = true;
	static_cast<CSilvermane*>(m_pOwner)->Return_Shield();
	m_pTransform->Rotation_Axis(CTransform::STATE_LOOK, XMConvertToRadians(-45.f));

	Spline_Return();
	m_fAccTime = 0.f;

	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);
}

void CFlyingShield::Spline_Throw()
{
	_vector p0{};
	_vector p1 = XMVectorSetW(m_pTransform->Get_State(CTransform::STATE_POSITION), 0.f); /* start */
	_vector p2{}; /* curve */
	_vector p3 = XMLoadFloat3(&m_tDesc.vTargetPos); /* end */
	_vector p4{};

	_float dist = MathUtils::Length(p3, p1);
	_float curveFactor = dist * 0.05f;
	_vector temp = p3 - p1;
	temp /= 2;
	temp += (XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)) * 0.f +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * curveFactor +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * 0.f);
	p2 = p1 + temp;
	p0 = p1 + (MathUtils::Direction(p2, p1));
	p4 = p3 + (MathUtils::Direction(p2, p3));

	p0 = XMVectorSetW(p0, 1.f);
	p1 = XMVectorSetW(p1, 1.f);
	p2 = XMVectorSetW(p2, 1.f);
	p3 = XMVectorSetW(p3, 1.f);
	p4 = XMVectorSetW(p4, 1.f);

	m_pSpline->DeletePoints();
	m_pSpline->AddPoint(p0);
	m_pSpline->AddPoint(p1);
	m_pSpline->AddPoint(p2);
	m_pSpline->AddPoint(p3);
	m_pSpline->AddPoint(p4);
}

void CFlyingShield::Spline_Return()
{
	_vector p0{};
	_vector p1 = XMLoadFloat3(&m_tDesc.vTargetPos); /* start */
	_vector p2{}; /* curve */
	_vector p3 = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, 0.f, -1.f }; /* end */
	_vector p4{};

	_float dist = MathUtils::Length(p3, p1);
	_float curveFactor = dist * 0.1f;
	_vector temp = p3 - p1;
	temp /= 2;
	temp += (XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)) * 0.f +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * curveFactor +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * 0.f);
	p2 = p1 + temp;
	p0 = p1 + (MathUtils::Direction(p2, p1));
	p4 = p3 + (MathUtils::Direction(p2, p3));

	p0 = XMVectorSetW(p0, 1.f);
	p1 = XMVectorSetW(p1, 1.f);
	p2 = XMVectorSetW(p2, 1.f);
	p3 = XMVectorSetW(p3, 1.f);
	p4 = XMVectorSetW(p4, 1.f);

	m_pSpline->DeletePoints();
	m_pSpline->AddPoint(p0);
	m_pSpline->AddPoint(p1);
	m_pSpline->AddPoint(p2);
	m_pSpline->AddPoint(p3);
	m_pSpline->AddPoint(p4);
}

CFlyingShield* CFlyingShield::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CFlyingShield* pInstance = new CFlyingShield(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CFlyingShield Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFlyingShield::Clone(_uint _iSceneID, void* _pArg)
{
	CFlyingShield* pInstance = new CFlyingShield(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CFlyingShield Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlyingShield::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Delete(m_pSpline);
}

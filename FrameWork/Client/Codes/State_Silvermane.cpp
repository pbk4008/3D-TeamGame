#include "pch.h"
#include "State_Silvermane.h"

#include "HierarchyNode.h"
#include "Bullet.h"

CState_Silvermane::CState_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CState(_pDevice, _pDeviceContext)
{
}

HRESULT CState_Silvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CState_Silvermane::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	iProgress = Input(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CState_Silvermane::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CState_Silvermane::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Silvermane::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Silvermane::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;
	
	m_isShake = false;

	return S_OK;
}

void CState_Silvermane::Set_Model(CModel* _pModel)
{
	m_pModel = _pModel;
}

void CState_Silvermane::Set_AnimationController(CAnimationController* _pAnimationController)
{
	m_pAnimationController = _pAnimationController;
}

const _int CState_Silvermane::Add_PlusAngle(const EDir _eDir, const _double& _dDeltaTime)
{
	_float fPlusAngle = m_pSilvermane->Get_PlusAngle();
	_float fValue = 5.f; // 고정시킬 각도의 여유를 주는값
	switch (_eDir)
	{
	case EDir::Forward:
		if (0.f - fValue > fPlusAngle && -180.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (0.f + fValue < fPlusAngle && 180.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-180.f >= fPlusAngle && -360.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (180.f <= fPlusAngle && 360.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::LeftForward:
		if (-45.f + fValue < fPlusAngle && 135.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-45.f - fValue > fPlusAngle && -225.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (135.f <= fPlusAngle && 315.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-225.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (315.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::RightForward:
		if (45.f - fValue > fPlusAngle && -135.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (45.f + fValue < fPlusAngle && 225.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (-135.f >= fPlusAngle && -315.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (225.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-315.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::Backward:
		if (0.f >= fPlusAngle && -180.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (0.f <= fPlusAngle && 180.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-180.f - fValue > fPlusAngle && -360.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (180.f + fValue < fPlusAngle && 360.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::LeftBackward:
		if (45.f >= fPlusAngle && -135.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (45.f <= fPlusAngle && 225.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-135.f - fValue > fPlusAngle && -315.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-315.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (225.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::RightBackward:
		if (-45.f <= fPlusAngle && 135.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-45.f >= fPlusAngle && -225.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (135.f + fValue < fPlusAngle && 315.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (315.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-225.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	case EDir::Left:
		if (90.f >= fPlusAngle && -90.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (90.f <= fPlusAngle && 270.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-90.f - fValue > fPlusAngle && -270.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-270.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (270.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		break;
	case EDir::Right:
		if (-90.f <= fPlusAngle && 90.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-90.f >= fPlusAngle && -270.f + fValue < fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (90.f + fValue < fPlusAngle && 270.f >= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)-_dDeltaTime);
		}
		else if (270.f <= fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		else if (-270.f - fValue > fPlusAngle)
		{
			m_pSilvermane->Add_PlusAngle((_float)_dDeltaTime);
		}
		break;
	}

	return _int();
}

const _int CState_Silvermane::ToIdle()
{
	if (m_pSilvermane->IsEquipWeapon())
	{
		switch (m_pSilvermane->Get_WeaponType())
		{
		case CWeapon::EType::Sword_1H:
			if (FAILED(m_pStateController->Change_State(L"1H_SwordIdle")))
				return -1;
			break;
		case CWeapon::EType::Hammer_2H:
			if (FAILED(m_pStateController->Change_State(L"2H_HammerIdle")))
				return -1;
			break;
		}
		return STATE_CHANGE;
	}
	else
	{
		if (FAILED(m_pStateController->Change_State(L"Idle")))
			return -1;
		return STATE_CHANGE;
	}

	return _int();
}

const _int CState_Silvermane::ToSprint()
{
	if (!m_pSilvermane->IsEquipWeapon())
	{
		if (FAILED(m_pStateController->Change_State(L"SprintFwdStart")))
			return -1;
	}
	else
	{
		switch (m_pSilvermane->Get_WeaponType())
		{
		case CWeapon::EType::Sword_1H:
			if (FAILED(m_pStateController->Change_State(L"1H_SwordEquipOff")))
				return -1;
			break;
		case CWeapon::EType::Hammer_2H:
			if (FAILED(m_pStateController->Change_State(L"2H_HammerEquipOff")))
				return -1;
			break;
		}
	}
	return STATE_CHANGE;
}

const _int CState_Silvermane::ToJogAttack()
{
	switch (m_pSilvermane->Get_WeaponType())
	{
	case CWeapon::EType::Sword_1H:
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogAttack")))
			return -1;
		break;
	case CWeapon::EType::Hammer_2H:
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackJogR1")))
			return -1;
		break;
	}
	return STATE_CHANGE;
}

const _int CState_Silvermane::ToDashAttack()
{
	switch (m_pSilvermane->Get_WeaponType())
	{
	case CWeapon::EType::Sword_1H:
		if (FAILED(m_pStateController->Change_State(L"1H_SwordJogAttack")))
			return -1;
		break;
	case CWeapon::EType::Hammer_2H:
		if (FAILED(m_pStateController->Change_State(L"2H_HammerAttackDodgeR1")))
			return -1;
		break;
	}
	return STATE_CHANGE;
}

const _int CState_Silvermane::ToChargeStart()
{
	switch (m_pSilvermane->Get_WeaponType())
	{
	case CWeapon::EType::Sword_1H:
		if (FAILED(m_pStateController->Change_State(L"1H_SwordAttackNormalR2_Start")))
			return -1;
		break;
	case CWeapon::EType::Hammer_2H:
		if (FAILED(m_pStateController->Change_State(L"2H_HammerChargeStage1_Start")))
			return -1;
		break;
	}
	return STATE_CHANGE;
}

void CState_Silvermane::Set_Silvermane(CSilvermane* _pSilvermane)
{
	m_pSilvermane = _pSilvermane;
}

void CState_Silvermane::Set_Transform(CTransform* _pTransform)
{
	m_pTransform = _pTransform;
}

_int CState_Silvermane::Input(const _double& _dDeltaTime)
{
	return _int();
}

void CState_Silvermane::Reflect_Bullet(const ATTACKDESC& _tAttackDesc)
{
	CTransform* pTargetTransform = _tAttackDesc.pOwner->Get_Transform();
	CTransform* pBulletTransform = _tAttackDesc.pHitObject->Get_Transform();

	CHierarchyNode* pBulletNode = m_pModel->Get_Bone("weapon_l");
	_matrix smatBullet = pBulletNode->Get_TransformMatrix();
	_matrix smatPivot = m_pModel->Get_PivotMatrix();
	_matrix smatWorld = m_pTransform->Get_WorldMatrix();
	smatBullet *= smatPivot * smatWorld;

	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - pTargetTransform->Get_State(CTransform::STATE_POSITION));
	_vector svRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), svLook));
	_vector svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));
	svRight *= pBulletTransform->Get_Scale(CTransform::STATE_RIGHT);
	svUp *= pBulletTransform->Get_Scale(CTransform::STATE_UP);
	svLook *= pBulletTransform->Get_Scale(CTransform::STATE_LOOK);

	memcpy_s(&smatBullet.r[0], sizeof(_vector), &svRight, sizeof(_vector));
	memcpy_s(&smatBullet.r[1], sizeof(_vector), &svUp, sizeof(_vector));
	memcpy_s(&smatBullet.r[2], sizeof(_vector), &svLook, sizeof(_vector));
	_uint iSceneID = g_pGameInstance->getCurrentLevel();
	CBullet* pBullet = nullptr;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Bullet", L"Proto_GameObject_Shooter_Bullet", &smatBullet, (CGameObject**)&pBullet)))
	{
		MSGBOX(L"패링 총알생성 실패!");
		return;
	}
	pBullet->Set_Owner(m_pSilvermane);
}

void CState_Silvermane::Hit(const ATTACKDESC& _tAttackDesc)
{
	if (m_pSilvermane->IsHit() || m_pSilvermane->IsAttack() || m_pSilvermane->Get_Dead())
		return;

	m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);

	if (0.f >= m_pSilvermane->Get_CurrentHp())
	{
		Death(_tAttackDesc);
		return;
	}

	EDir eDirFB = EDir::Max;
	EDir eDirLR = EDir::Max;
	_vector svDir = XMVector3Normalize(XMVectorSetY(_tAttackDesc.pOwner->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION), 0.f));
	_vector svLook = XMVector3Normalize(XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_LOOK), 0.f));
	// 내적으로 앞뒤 판별
	_vector svDot = XMVector3Dot(svLook, svDir);
	if (0.f < XMVectorGetX(svDot))
		eDirFB = EDir::Forward;
	else
		eDirFB = EDir::Backward;
	// 외적으로 좌우 판별
	_vector svCross = XMVector3Cross(svLook, svDir);
	if (0.f < XMVectorGetY(svCross))
		eDirLR = EDir::Right;
	else
		eDirLR = EDir::Left;

	if (EDir::Backward == eDirFB && 1 < _tAttackDesc.iLevel)
	{
		m_pStateController->Change_State(L"KnockBack", &eDirFB);
		return;
	}

	switch (_tAttackDesc.iLevel)
	{
	case 1:
		m_pStateController->Change_State(L"1H_FlinchLeft", &eDirLR);
		return;
		break;
	case 2:
		m_pStateController->Change_State(L"1H_Stagger", &eDirLR);
		return;
		break;
	case 3:
		m_pStateController->Change_State(L"1H_KnockBack");
		return;
		break;
	case 4:
		m_pStateController->Change_State(L"KnockBack", &eDirFB);
		return;
		break;
	}
}

void CState_Silvermane::Block(const ATTACKDESC& _tAttackDesc)
{
	if (m_pSilvermane->IsHit() || m_pSilvermane->Get_Dead())
		return;

	EDir eDirFB = EDir::Max;
	_vector svDir = XMVector3Normalize(XMVectorSetY(_tAttackDesc.pOwner->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION), 0.f));
	_vector svLook = XMVector3Normalize(XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_LOOK), 0.f));
	// 내적으로 앞뒤 판별
	_vector svDot = XMVector3Dot(svLook, svDir);
	if (0.f < XMVectorGetX(svDot))
		eDirFB = EDir::Forward;
	else
		eDirFB = EDir::Backward;
	
	_float fBlockTime = m_pSilvermane->Get_BlockTime();
	if (EDir::Backward == eDirFB && 1 < _tAttackDesc.iLevel)
	{
		m_pSilvermane->Set_EquipShield(false);
		m_pSilvermane->Set_EquipShieldAnim(false);
		m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);

		if (0.f < m_pSilvermane->Get_CurrentHp())
			m_pStateController->Change_State(L"KnockBack", &eDirFB);
		else
			Death(_tAttackDesc);
		return;
	}

	// 아직 패링이 가능해!
	if (0.5f > fBlockTime)
	{
		switch (_tAttackDesc.iLevel)
		{
		case 1:
		case 2:
			if ((_uint)GAMEOBJECT::WEAPON_BULLET == _tAttackDesc.pHitObject->getTag())
			{
				Reflect_Bullet(_tAttackDesc);
			}
			m_pStateController->Change_State(L"Shield_Parry");
			return;
			break;
		case 3:
			m_pStateController->Change_State(L"Shield_ParryStunback");
			return;
			break;
		case 4:
			m_pStateController->Change_State(L"Shield_ParryStunbackStrong");
			return;
			break;
		}
	}
	else
	{
		switch (_tAttackDesc.iLevel)
		{
		case 1:
			m_pStateController->Change_State(L"Shield_BlockSkid");
			return;
			break;
		case 2:
			m_pStateController->Change_State(L"Shield_BlockSkidStrong");
			return;
			break;
		case 3:
			m_pStateController->Change_State(L"Shield_Ricochet");
			return;
			break;
		case 4:
			m_pStateController->Change_State(L"Shield_BlockBreakStart");
			return;
			break;
		}
	}

	//m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
}

void CState_Silvermane::Death(const ATTACKDESC& _tAttackDesc)
{
	if (m_pSilvermane->Get_Dead())
		return;

	_vector svDir = XMVector3Normalize(XMVectorSetY(_tAttackDesc.pOwner->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransform->Get_State(CTransform::STATE_POSITION), 0.f));
	_vector svLook = XMVector3Normalize(XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_LOOK), 0.f));

	_vector svAngle = XMVector3AngleBetweenVectors(svLook, svDir);
	_float fRadian = 0.f;
	_float fAngle = 0.f;
	XMStoreFloat(&fRadian, svAngle);
	_vector svCross = XMVector3Cross(svLook, svDir);
	if (0.f < XMVectorGetY(svCross)) // 타겟이 오른쪽
	{
		fAngle = XMConvertToDegrees(fRadian);
	}
	else // 타겟이 왼쪽
	{
		fAngle = -XMConvertToDegrees(fRadian);
	}

	EDir eDir = EDir::Max;
	if (-45.f <= fAngle && 45.f >= fAngle) // Fwd
	{
		eDir = EDir::Forward;
	}
	else if (-45.f > fAngle && -135.f < fAngle) // Left
	{
		eDir = EDir::Left;
	}
	else if (45.f < fAngle && 135.f > fAngle) // Right
	{
		eDir = EDir::Right;
	}
	else // Bwd
	{
		eDir = EDir::Backward;
	}
	if (FAILED(m_pStateController->Change_State(L"Death", &eDir)))
		return;
}

void CState_Silvermane::Free()
{

	__super::Free();
}

#include "pch.h"
#include "State_Silvermane.h"

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

void CState_Silvermane::OnTriggerEnterHit(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();

	switch (iTag)
	{
		// 무기들과 충돌했을 경우
	case (_uint)GAMEOBJECT::WEAPON_MIDBOSS:
	case (_uint)GAMEOBJECT::WEAPON_EARTH:
	case (_uint)GAMEOBJECT::WEAPON_BRONZE:
	case (_uint)GAMEOBJECT::WEAPON_2HSword:
	case (_uint)GAMEOBJECT::WEAPON_POLEARM:
		if (static_cast<CWeapon*>(collision.pGameObject)->IsAttack())
		{
			ATTACKDESC tAttackDesc = static_cast<CWeapon*>(collision.pGameObject)->Get_Owner()->Get_AttackDesc();
			tAttackDesc.fDamage += static_cast<CWeapon*>(collision.pGameObject)->Get_Damage();
			m_pSilvermane->Add_HP(-tAttackDesc.fDamage);
			switch (tAttackDesc.iLevel)
			{
			case 1:
				m_pStateController->Change_State(L"1H_FlinchLeft");
				return;
				break;
			case 2:

				break;
			case 3:

				break;
			}
		}
		break;
		// 투사체들과 충돌했을 경우, 총알 or 마법 or 레이저
	case (_uint)GAMEOBJECT::WEAPON_BULLET:
	{
		ATTACKDESC tAttackDesc = static_cast<CWeapon*>(collision.pGameObject)->Get_Owner()->Get_AttackDesc();
		tAttackDesc.fDamage += static_cast<CWeapon*>(collision.pGameObject)->Get_Damage();
		m_pSilvermane->Add_HP(-tAttackDesc.fDamage);
		switch (tAttackDesc.iLevel)
		{
		case 1:
			m_pStateController->Change_State(L"1H_FlinchLeft");
			return;
			break;
		case 2:

			break;
		case 3:

			break;
		}
	}
		break;
		// 몬스터들과 직접 충돌햇을 경우
	case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
		if (static_cast<CActor*>(collision.pGameObject)->IsAttack())
		{
			ATTACKDESC tAttackDesc = static_cast<CActor*>(collision.pGameObject)->Get_AttackDesc();
			m_pSilvermane->Add_HP(-tAttackDesc.fDamage);
			switch (tAttackDesc.iLevel)
			{
			case 1:
				m_pStateController->Change_State(L"1H_FlinchLeft");
				return;
				break;
			case 2:

				break;
			case 3:

				break;
			}
		}
		break;
	}
}

void CState_Silvermane::Hit(const ATTACKDESC& _tAttackDesc)
{
#pragma region 플레이어에서만 충돌 처리를 해줄 경우
	//_uint iTag = _tAttackDesc.pGameObject->getTag();

	//switch (iTag)
	//{
	//	// 무기들과 충돌했을 경우
	//case (_uint)GAMEOBJECT::WEAPON_MIDBOSS:
	//case (_uint)GAMEOBJECT::WEAPON_EARTH:
	//case (_uint)GAMEOBJECT::WEAPON_BRONZE:
	//case (_uint)GAMEOBJECT::WEAPON_2HSword:
	//case (_uint)GAMEOBJECT::WEAPON_POLEARM:
	//	if (static_cast<CWeapon*>(_tAttackDesc.pGameObject)->IsAttack())
	//	{
	//		m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//		switch (_tAttackDesc.iLevel)
	//		{
	//		case 1:
	//			m_pStateController->Change_State(L"1H_FlinchLeft");
	//			return;
	//			break;
	//		case 2:

	//			break;
	//		case 3:

	//			break;
	//		}
	//	}
	//	break;
	//	// 투사체들과 충돌했을 경우, 총알 or 마법 or 레이저
	//case (_uint)GAMEOBJECT::WEAPON_BULLET:
	//{
	//	m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//	switch (_tAttackDesc.iLevel)
	//	{
	//	case 1:
	//		m_pStateController->Change_State(L"1H_FlinchLeft");
	//		return;
	//		break;
	//	case 2:

	//		break;
	//	case 3:

	//		break;
	//	}
	//}
	//break;
	//// 몬스터들과 직접 충돌햇을 경우
	//case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
	//	if (static_cast<CActor*>(_tAttackDesc.pGameObject)->IsAttack())
	//	{
	//		m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//		switch (_tAttackDesc.iLevel)
	//		{
	//		case 1:
	//			m_pStateController->Change_State(L"1H_FlinchLeft");
	//			return;
	//			break;
	//		case 2:

	//			break;
	//		case 3:

	//			break;
	//		}
	//	}
	//	break;
	//}
#pragma endregion

	m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	switch (_tAttackDesc.iLevel)
	{
	case 1:
		m_pStateController->Change_State(L"1H_FlinchLeft");
		return;
		break;
	case 2:
		//m_pStateController->Change_State(L"1H_KnockBack_Land");
		m_pStateController->Change_State(L"KnockBackBwd");
		return;
		break;
	case 3:

		break;
	}
}

void CState_Silvermane::Block(const ATTACKDESC& _tAttackDesc)
{
#pragma region 플레이어에서만 충돌 처리를 해줄 경우
	//_uint iTag = _tAttackDesc.pGameObject->getTag();

	//switch (iTag)
	//{
	//	// 무기들과 충돌했을 경우
	//case (_uint)GAMEOBJECT::WEAPON_MIDBOSS:
	//case (_uint)GAMEOBJECT::WEAPON_EARTH:
	//case (_uint)GAMEOBJECT::WEAPON_BRONZE:
	//case (_uint)GAMEOBJECT::WEAPON_2HSword:
	//case (_uint)GAMEOBJECT::WEAPON_POLEARM:
	//	if (static_cast<CWeapon*>(_tAttackDesc.pGameObject)->IsAttack())
	//	{
	//		//m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//		switch (_tAttackDesc.iLevel)
	//		{
	//		case 1:
	//			m_pStateController->Change_State(L"Shield_BlockSkid");
	//			return;
	//			break;
	//		case 2:

	//			break;
	//		case 3:

	//			break;
	//		}
	//	}
	//	break;
	//	// 투사체들과 충돌했을 경우, 총알 or 마법 or 레이저
	//case (_uint)GAMEOBJECT::WEAPON_BULLET:
	//{
	//	//m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//	switch (_tAttackDesc.iLevel)
	//	{
	//	case 1:
	//		m_pStateController->Change_State(L"Shield_BlockSkid");
	//		return;
	//		break;
	//	case 2:

	//		break;
	//	case 3:

	//		break;
	//	}
	//}
	//break;
	//// 몬스터들과 직접 충돌햇을 경우
	//case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
	//	if (static_cast<CActor*>(_tAttackDesc.pGameObject)->IsAttack())
	//	{
	//		//m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	//		switch (_tAttackDesc.iLevel)
	//		{
	//		case 1:
	//			m_pStateController->Change_State(L"Shield_BlockSkid");
	//			return;
	//			break;
	//		case 2:

	//			break;
	//		case 3:

	//			break;
	//		}
	//	}
	//	break;
	//}
#pragma endregion

	//m_pSilvermane->Add_HP(-_tAttackDesc.fDamage);
	switch (_tAttackDesc.iLevel)
	{
	case 1:
		m_pStateController->Change_State(L"Shield_BlockSkid");
		return;
		break;
	case 2:

		break;
	case 3:

		break;
	}
}

void CState_Silvermane::Free()
{

	__super::Free();
}

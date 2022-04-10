#include "pch.h"
#include "Bastion_Healer_Attack.h"
#include "Animation.h"

/* Monster List */
#include "Monster_Bastion_Healer.h"

CBastion_Healer_Attack::CBastion_Healer_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CBastion_Healer_State(_pDevice, _pDeviceContext)
	, m_bSpawn(false)
{
}

CBastion_Healer_Attack::CBastion_Healer_Attack(const CBastion_Healer_Attack& _rhs)
	: CBastion_Healer_State(_rhs)
	, m_bSpawn(false)
{
}

HRESULT CBastion_Healer_Attack::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBastion_Healer_Attack::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Check_Attack();
	m_pAnimator->Tick(_dDeltaTime);
	if (m_pAnimator->Get_CurrentAnimation()->Is_Finished())
	{
		m_bSpawn = false;
		m_pOwner->set_Attack(false);
		m_pStateController->Change_State(L"Idle");
	}

	return _int();
}

_int CBastion_Healer_Attack::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBastion_Healer_Attack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBastion_Healer_Attack::EnterState()
{
	if (FAILED(__super::EnterState()))
		return E_FAIL;

	m_pTransform->Face_Target(g_pObserver->Get_PlayerPos());
	m_pAnimator->Change_AnyEntryAnimation((_uint)CMonster_Bastion_Healer::ANIM_TYPE::A_ATTACK_BLIND);

	return S_OK;
}

HRESULT CBastion_Healer_Attack::ExitState()
{
	if (FAILED(__super::ExitState()))
		return E_FAIL;

	return S_OK;
}

void CBastion_Healer_Attack::Check_Attack()
{
	_uint iCurAnimFrame = m_pAnimator->Get_CurrentAnimation()->Get_CurrentKeyFrameIndex();
	//cout << iCurAnimFrame << endl;
	if (iCurAnimFrame >= 160 && !m_bSpawn)
	{
		m_bSpawn = true;
		_uint iSceneID = g_pGameInstance->getCurrentLevel();

		_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		vPos=XMVectorSetY(vPos, 10.f);
		
		_vector vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		vLook=XMVector3Normalize(vLook);
		vLook *= -30.f;
		vPos += vLook;


		vector<_vector> vecRand;
		while (true)
		{
			_uint iVecSize = vecRand.size();
			if (iVecSize > 5)
				break;
			_float fSizeX = MathUtils::ReliableRandom(-30.f, 30.f);
			_float fSizeY = MathUtils::ReliableRandom(0.f, 5.f);
			_vector vRand = XMVectorSet(fSizeX, fSizeY, 0.f, 0.f);
			vPos += vRand;
			_bool bCheck = false;
			for (auto& vRandPos : vecRand)
			{
				_float fLen = XMVectorGetX(XMVector3Length(vPos - vRandPos));
				if (fLen < 6)
				{
					bCheck = true;
					break;
				}
			}
			if (bCheck)
				continue;
			vecRand.emplace_back(vPos);
		}
		
		for (_uint i = 0; i < 5; i++)
		{
			g_pGameInstance->Add_GameObjectToLayer(iSceneID, L"Layer_Potal", L"Proto_GameObject_Portal", &vecRand[i]);
		}
	}
}

void CBastion_Healer_Attack::Look_Player(void)
{
	CAnimation* pAnim = m_pAnimator->Get_CurrentAnimation();

	if (pAnim->Is_Finished())
		m_pStateController->Change_State(L"Idle");
}

void CBastion_Healer_Attack::Look_Monster(void)
{

}

CBastion_Healer_Attack* CBastion_Healer_Attack::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg)
{
	CBastion_Healer_Attack* pInstance = new CBastion_Healer_Attack(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBastion_Healer_Attack Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBastion_Healer_Attack::Free()
{
	__super::Free();
}

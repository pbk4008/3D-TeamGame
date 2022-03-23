#include "pch.h"
#include "Client_Observer.h"

CClient_Observer::CClient_Observer(void)
	: m_pPlayer(nullptr)
{
}

HRESULT CClient_Observer::Set_Player(CSilvermane* pPlayer)
{
	if (m_pPlayer)
		Safe_Release(m_pPlayer);

	m_pPlayer = pPlayer;
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

const CTransform* CClient_Observer::Get_Transform()
{
	if (!m_pPlayer)
		return nullptr;

	return m_pPlayer->Get_Transform();
}

_fmatrix CClient_Observer::Get_PlayerWorldMatrix()
{
	_matrix matPlayerWorld;
	ZeroMemory(&matPlayerWorld, sizeof(_matrix));
	//플레이어가 없으면
	if (!m_pPlayer)
		return matPlayerWorld;//0행렬 밖으로

	//Transform 가져오기
	const CTransform* pTransform = Get_Transform();
	//Transform이 없으면
	if (!pTransform)
		return matPlayerWorld;//0행렬

	matPlayerWorld = pTransform->Get_WorldMatrix();//월드 행렬 가져오기

	return matPlayerWorld;
}

_fvector CClient_Observer::Get_PlayerPos()
{
	_matrix matTransform = Get_PlayerWorldMatrix();

	_vector vPos = matTransform.r[3];

	return vPos;
}

const _bool CClient_Observer::IsAttack()
{
	if (!m_pPlayer)
		return false;

	return m_pPlayer->IsAttack();
}

const _float CClient_Observer::Get_Dist(_fvector vPos)
{
	//플레이어 위치 받아오기
	_vector vPlayer = Get_PlayerPos();
	//플레이어의 좌표가 0일경우 -1 리턴
	if (XMVector4Equal(vPlayer, XMVectorZero()))
		return -1;

	//거리 구하기
	_float fDist = XMVectorGetX(XMVector3Length(vPlayer - vPos));

	return fDist;
}

const _float CClient_Observer::Get_HP()
{
	return m_pPlayer->Get_CurrentHp();
}

const CSilvermane::SCENEMOVEDATA CClient_Observer::Get_SceneMoveData()
{
	return m_pPlayer->Get_SceneMoveData();
}

void CClient_Observer::Set_IsAttack(const _bool bAttack)
{
	if (!m_pPlayer)
		return;

	m_pPlayer->Set_IsAttack(bAttack);
}

void CClient_Observer::Free(void)
{
	Safe_Release(m_pPlayer);
}

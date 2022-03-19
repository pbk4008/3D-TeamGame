#include "pch.h"
#include "Client_Observer.h"
#include "Silvermane.h"

CClient_Observer::CClient_Observer(void)
	: m_pPlayer(nullptr)
{
}

HRESULT CClient_Observer::Set_Player(CSilvermane* pPlayer)
{
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

const _bool CClient_Observer::Get_IsAttack()
{
	if (!m_pPlayer)
		return false;

	return m_pPlayer->Get_IsAttack();
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

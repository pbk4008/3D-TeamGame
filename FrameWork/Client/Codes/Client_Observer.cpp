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
	//�÷��̾ ������
	if (!m_pPlayer)
		return matPlayerWorld;//0��� ������

	//Transform ��������
	const CTransform* pTransform = Get_Transform();
	//Transform�� ������
	if (!pTransform)
		return matPlayerWorld;//0���

	matPlayerWorld = pTransform->Get_WorldMatrix();//���� ��� ��������

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

const _float CClient_Observer::Get_Dist(_fvector vPos)
{
	//�÷��̾� ��ġ �޾ƿ���
	_vector vPlayer = Get_PlayerPos();
	//�÷��̾��� ��ǥ�� 0�ϰ�� -1 ����
	if (XMVector4Equal(vPlayer, XMVectorZero()))
		return -1;

	//�Ÿ� ���ϱ�
	_float fDist = XMVectorGetX(XMVector3Length(vPlayer - vPos));

	return fDist;
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

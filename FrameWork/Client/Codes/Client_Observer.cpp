#include "pch.h"
#include "Client_Observer.h"

CClient_Observer::CClient_Observer(void)
	: m_pPlayer(nullptr)
	, m_bAttackAnimStart(false)
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

const _bool CClient_Observer::IsAttack()
{
	if (!m_pPlayer)
		return false;

	return m_pPlayer->IsAttack();
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

const _float CClient_Observer::Get_HP()
{
	return m_pPlayer->Get_CurrentHp();
}

const _float CClient_Observer::Get_MaxHP()
{
	return m_pPlayer->Get_MaxHp();
}

const _float CClient_Observer::Get_HPRatio()
{
	return m_pPlayer->Get_HpRatio();
}

void CClient_Observer::Set_IsThrownObject(const _bool _isThrownObject)
{
	m_isThrownObject = _isThrownObject;
}

const _bool CClient_Observer::IsThrownObject()
{
	return m_isThrownObject;
}

const CSilvermane::SCENEMOVEDATA CClient_Observer::Get_SceneMoveData()
{
	return m_pPlayer->Get_SceneMoveData();
}

const _bool CClient_Observer::Get_PlayerAttackAnimStart()
{
	return m_bAttackAnimStart;
}

const _bool CClient_Observer::Get_PlayerFrontBackCheck(_fvector vPos)
{
	_vector vPlayerPos = Get_PlayerPos();
	_vector vDir = vPos - vPlayerPos;
	vDir = XMVector3Normalize(vDir);

	_matrix matMatrix = Get_PlayerWorldMatrix();
	_vector vLook = XMVector3Normalize(matMatrix.r[2]);

	_float fDot = XMVectorGetX(XMVector3Dot(vDir, vLook));

	if (fDot < 0.f)
		return false;

	return true;
}

_fvector CClient_Observer::Get_Dir(_fvector vPos)
{
	_vector vPlayerPos = Get_PlayerPos();
	_vector vDir = vPos - vPlayerPos;
	vDir = XMVector3Normalize(vDir);

	return vDir;
}

void CClient_Observer::Player_Active(_bool bCheck)
{
	if (!m_pPlayer)
		return;
	m_pPlayer->setActive(bCheck);
}

void CClient_Observer::Set_Exp(_float fExp)
{
	if (!m_pPlayer)
		return;

	m_bLevelUp = false;

	if (true == m_pPlayer->Get_PlayerData()->SetExp(fExp))
	{
		m_bLevelUp = true;
		g_pInvenUIManager->ShowLevelUpHUD(m_pPlayer->Get_PlayerData()->Get_Level());
	}
}

void CClient_Observer::Set_IsAttack(const _bool bAttack)
{
	if (!m_pPlayer)
		return;

	m_pPlayer->Set_IsAttack(bAttack);
}

void CClient_Observer::Set_Hp(const _float fCurrentHp)
{
	if (!m_pPlayer)
		return;
	m_pPlayer->Set_CurrentHp(fCurrentHp);
}

void CClient_Observer::MinusHp(const _float fDamage)
{
	if (!m_pPlayer)
		return;

	_float fCurrentHp = Get_HP();
	
	fCurrentHp -= fDamage;
	Set_Hp(fCurrentHp);
}

_bool CClient_Observer::Get_PlayerActive()
{
	if (!m_pPlayer)
		return false;

	return m_pPlayer->getActive();
}

void CClient_Observer::Free(void)
{
	Safe_Release(m_pPlayer);
}

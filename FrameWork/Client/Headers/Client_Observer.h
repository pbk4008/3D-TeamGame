#ifndef Client_Observer_h__
#define Client_Observer_h__

#include "Silvermane.h"
#include "SingleTon.h"
BEGIN(Client)
class CClient_Observer final :  public CSingleTon<CClient_Observer>
{
friend CSingleTon;
private:
	explicit CClient_Observer(void);
	virtual ~CClient_Observer(void) = default;

public:
	//�÷��̾� ����
	HRESULT Set_Player(CSilvermane* pPlayer);
public:
	//�÷��̾� Transform��������
	const CTransform* Get_Transform();
	//�÷��̾� ����Matrix ��������
	_fmatrix Get_PlayerWorldMatrix();
	//�÷��̾� Pos ��������
	_fvector Get_PlayerPos();
	//�÷��̾ �����ϴ��� ���ϴ��� üũ
	const _bool IsAttack();
	//�÷��̾�� �ش� ��ǥ���� �Ÿ�
	const _float Get_Dist(_fvector vPos);
	//�÷��̾��� ����HP ��������
	const _float Get_HP();
	//�÷��̾��� �ִ�HP ��������
	const _float Get_MaxHP();
	//�÷��̾��� �ִ�ü�°� ����ü���� ���� ��������
	const _float Get_HPRatio();
	//�÷��̾� �� �Ѿ�� �״�� ���������� �����͵� 
	const CSilvermane::SCENEMOVEDATA Get_SceneMoveData();
	// �÷��̾ �����ų� �и��� ���� �ݻ��ϴ� ������Ʈ�� �ִ��� ����
	void Set_IsThrownObject(const _bool _isThrownObject);
	const _bool IsThrownObject();
	//�÷��̾��� ���� �ִϸ��̼� �����ϴ��� 
	const _bool Get_PlayerAttackAnimStart();
	//�ش� ��ǥ�� �÷��̾ �ٶ󺸴� �������� �տ� �ִ��� �ڿ� �ִ��� �Ǻ�
	const _bool Get_PlayerFrontBackCheck(_fvector vPos);
	//�ش� ��ǥ������ ���⺤��
	_fvector Get_Dir(_fvector vPos);
	void Player_Active(_bool bCheck);
public:
	//�÷��̾� ���� ����
	void Set_IsAttack(const _bool bAttack);
	void Set_Hp(const _float fCurrentHp);
	void MinusHp(const _float fDamage);
	void Set_PlayerAttackAnimStart(const _bool bAnimStart) { m_bAttackAnimStart = bAnimStart; }
public:
	_bool Get_PlayerActive();
private:
	CSilvermane* m_pPlayer;
private: /* �������� ���� ������� */
	_bool m_isThrownObject = false;
	_bool m_bAttackAnimStart = false;
public:
	virtual void Free(void) override;
};
END
#endif // Client_Observer_h__
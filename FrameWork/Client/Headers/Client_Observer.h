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
	//�÷��̾��� HP ��������
	const _float Get_HP();

	const CSilvermane::SCENEMOVEDATA Get_SceneMoveData();
public:
	//�÷��̾� ���� ����
	void Set_IsAttack(const _bool bAttack);
	void Set_Hp(const _float fCurrentHp);
	void MinusHp(const _float fDamage);
private:
	CSilvermane* m_pPlayer;
public:
	virtual void Free(void) override;
};
END
#endif // Client_Observer_h__
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
	//플레이어 셋팅
	HRESULT Set_Player(CSilvermane* pPlayer);
public:
	//플레이어 Transform가져오기
	const CTransform* Get_Transform();
	//플레이어 월드Matrix 가져오기
	_fmatrix Get_PlayerWorldMatrix();
	//플레이어 Pos 가져오기
	_fvector Get_PlayerPos();
	//플레이어가 공격하는지 안하는지 체크
	const _bool IsAttack();
	//플레이어와 해당 좌표와의 거리
	const _float Get_Dist(_fvector vPos);
	//플레이어의 HP 가져오기
	const _float Get_HP();

	const CSilvermane::SCENEMOVEDATA Get_SceneMoveData();
public:
	//플레이어 공격 셋팅
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
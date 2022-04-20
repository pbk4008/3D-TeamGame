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
	//플레이어의 현재HP 가져오기
	const _float Get_HP();
	//플레이어의 최대HP 가져오기
	const _float Get_MaxHP();
	//플레이어의 최대체력과 현재체력의 비율 가져오기
	const _float Get_HPRatio();
	//플레이어 씬 넘어갈때 그대로 가져가야할 데이터들 
	const CSilvermane::SCENEMOVEDATA Get_SceneMoveData();
	// 플레이어가 던지거나 패링을 통해 반사하는 오브젝트가 있는지 세팅
	void Set_IsThrownObject(const _bool _isThrownObject);
	const _bool IsThrownObject();
	//플레이어의 공격 애니메이션 시작하는지 
	const _bool Get_PlayerAttackAnimStart();
	//해당 좌표가 플레이어가 바라보는 기준으로 앞에 있는지 뒤에 있는지 판별
	const _bool Get_PlayerFrontBackCheck(_fvector vPos);
	//해당 좌표까지의 방향벡터
	_fvector Get_Dir(_fvector vPos);
	void Player_Active(_bool bCheck);
public:
	//플레이어 공격 셋팅
	void Set_IsAttack(const _bool bAttack);
	void Set_Hp(const _float fCurrentHp);
	void MinusHp(const _float fDamage);
	void Set_PlayerAttackAnimStart(const _bool bAnimStart) { m_bAttackAnimStart = bAnimStart; }
public:
	_bool Get_PlayerActive();
private:
	CSilvermane* m_pPlayer;
private: /* 옵저버의 고유 멤버변수 */
	_bool m_isThrownObject = false;
	_bool m_bAttackAnimStart = false;
public:
	virtual void Free(void) override;
};
END
#endif // Client_Observer_h__
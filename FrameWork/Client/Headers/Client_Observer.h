#ifndef Client_Observer_h__
#define Client_Observer_h__

#include "SingleTon.h"
BEGIN(Client)
class CSilvermane;
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

private:
	CSilvermane* m_pPlayer;
public:
	virtual void Free(void) override;
};
END
#endif // Client_Observer_h__
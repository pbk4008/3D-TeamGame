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
	//�÷��̾� ����
	HRESULT Set_Player(CSilvermane* pPlayer);
public:
	//�÷��̾� Transform��������
	const CTransform* Get_Transform();
	//�÷��̾� ����Matrix ��������
	_fmatrix Get_PlayerWorldMatrix();
	//�÷��̾� Pos ��������
	_fvector Get_PlayerPos();

private:
	CSilvermane* m_pPlayer;
public:
	virtual void Free(void) override;
};
END
#endif // Client_Observer_h__
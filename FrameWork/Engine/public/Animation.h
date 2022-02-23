#pragma once

#include "Base.h"

/* �ִϸ��̼� �ϳ��� ����. */

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(char* pName, _double Duration, _double PlaySpeed);
	HRESULT Add_Channel(class CChannel* pChannel);
	HRESULT Update_TransformationMatrix(_double TimeDelta);

private:
	char				m_szName[MAX_PATH] = "";
	_double				m_Duration = 0.0; /*�ϳ��� �ִϸ��̼��� ����ϴµ� �ɸ��� �ð�(Ʈ���� ��ġ�� ����) */
	_double				m_TrackPositionAcc = 0.0;
	_double				m_PlaySpeed = 0.0;
	_bool				m_isFinished = false;

private:
	/* ���� �ִϸ��̼ǿ� ���⤷�� �ִ� ������� ����. */
	vector<class CChannel*>			m_Channels; 
	typedef vector<class CChannel*>	CHANNELS;
public:
	static CAnimation* Create(char* pName, _double Duration, _double PlaySpeed);
	virtual void Free();
};

END
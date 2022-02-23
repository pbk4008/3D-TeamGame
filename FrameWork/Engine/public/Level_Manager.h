#pragma once

#include "Base.h"

/* ���� �����ִ� ������ �ּҸ� ��������. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	/* ���������� ������. �����ְ���� �������� �����ϳ�.  */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pOpenLevel);
	_int Tick(_double TimeDelta);
	HRESULT Render();

private:
	_uint					m_iCurrentLevelID = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

private:
	HRESULT Clear_Managers();

public:
	virtual void Free() override;
};

END
#pragma once

#include "SingleTon.h"

/* 현재 보여주는 레벨의 주소를 보관핝다. */

BEGIN(Engine)

class CLevel_Manager final : public CSingleTon<CLevel_Manager>
{
	friend CSingleTon;
public:
	NO_COPY(CLevel_Manager);
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	/* 기존레벨을 날리고. 보여주고싶은 레베릉ㄹ 셋팅하낟.  */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pOpenLevel);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
	HRESULT Render();
	HRESULT Delete_GameObject();
public:
	_uint getCurrentLevel() { return m_iCurrentLevelID; }
	CLevel* getCurrentLevelScene() { return m_pCurrentLevel; }
private:
	_uint					m_iCurrentLevelID = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

private:
	HRESULT Clear_Managers();

public:
	virtual void Free() override;
};

END
#include "pch.h"
#include "Observer.h"

IMPLEMENT_SINGLETON(CObserver)

_fvector CObserver::Load_DebugingFile(const wstring& wstrFilePath /* = L"" */)
{
	if (L"" == wstrFilePath)
		return XMVectorZero();

	wifstream fin;
	fin.open(wstrFilePath);

	if (fin.fail())
		return XMVectorZero();

	TCHAR szUpdate[MAX_PATH]{};
	TCHAR szPosX[MAX_PATH]{};
	TCHAR szPosY[MAX_PATH]{};
	TCHAR szPosZ[MAX_PATH]{};
	TCHAR szCount[MAX_PATH]{};

	while (true)
	{
		
		fin.getline(szUpdate, MAX_PATH, L'||');

			return XMVectorZero();

		fin.getline(szPosX, MAX_PATH, L'|');
		fin.getline(szPosY, MAX_PATH, L'|');
		fin.getline(szPosZ, MAX_PATH, L'|');
		fin.getline(szCount, MAX_PATH);

		LPTSTR endPtr;
		_float fPosX = _tcstod(szPosX, &endPtr);
		_float fPosY = _tcstod(szPosY, &endPtr);
		_float fPosZ = _tcstod(szPosZ, &endPtr);

		m_DebugingPos.x = fPosX;
		m_DebugingPos.y = fPosY;
		m_DebugingPos.z = fPosZ;
		m_DebugingPos.w = 1.0f;

		if (fin.eof())
			break;
	
		int iCount = _wtoi(szCount);
	}
	fin.close();

	return XMLoadFloat4(&m_DebugingPos);
}

CObserver::CObserver()
{

}

void CObserver::Free(void)
{
}

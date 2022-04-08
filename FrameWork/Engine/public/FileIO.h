#ifndef FileIO_h__
#define FileIO_h__

#include "Engine_Defines.h"
#include <atlconv.h>

BEGIN(Engine)

class ENGINE_DLL CFileIO
{
public:
	CFileIO(void);
	~CFileIO(void);

public:
	static _int IsFileOrDir(_wfinddata_t fd);
	static void SearchingDir(std::list<std::wstring>* _fdList, const TCHAR* _path, const TCHAR* _extention);
	static void SearchingDir(std::list<std::wstring>* _fdList, const TCHAR* _path);
	static void SearchingFile(std::wstring* _fp, const TCHAR* _path, const std::wstring _name, const TCHAR* _extention);
};
END
#endif // FileIO_h__

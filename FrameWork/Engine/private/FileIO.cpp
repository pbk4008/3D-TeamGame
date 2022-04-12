#include "FileIO.h"
#include <atlconv.h>

CFileIO::CFileIO(void)
{
}

CFileIO::~CFileIO(void)
{
}

_int CFileIO::IsFileOrDir(_wfinddata_t fd)
{
	if (fd.attrib & _A_SUBDIR)
		return 0; /* ������ ��� 0 ���� */
	else
		return 1;

	return _int();
}

void CFileIO::SearchingDir(std::list<std::wstring>* _fdList, const TCHAR* _path, const TCHAR* _extention)
{
	std::wstring defaultPath = _path;
	std::wstring path = defaultPath + L"/*.*";
	std::wstring extention = _extention;
	_int checkDirFile = 0;
	struct _wfinddata_t fd;
	intptr_t handle;

	/* for. fd ����ü �ʱ�ȭ */
	if ((handle = _wfindfirst(path.c_str(), &fd)) == -1L)
	{
		// �����̳� ������ ã�� �� ���� ���
		_findclose(handle);
		return;
	}
	do /* ���� Ž�� �ݺ�*/
	{
		/* ���� ��ü ���� Ȯ�� (file or Dir) */
		checkDirFile = IsFileOrDir(fd);
		if (0 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			SearchingDir(_fdList, (defaultPath + L"/" + fd.name).c_str(), extention.c_str()); 
		}
		else if (1 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			std::wstring fileNmae = fd.name;
			std::wstring fileFormat = fileNmae.substr(fileNmae.find_last_of(L".") + 1);

			if (fileFormat == _extention)
			{
				std::wstring temp = defaultPath + L"/" + fileNmae;
				_fdList->emplace_back(temp.c_str());
			}
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}

void CFileIO::SearchingDir(std::list<std::wstring>* _fdList, const TCHAR* _path)
{
	std::wstring defaultPath = _path;
	std::wstring path = defaultPath + L"/*.*";
	_int checkDirFile = 0;
	struct _wfinddata_t fd;
	intptr_t handle;

	/* for. fd ����ü �ʱ�ȭ */
	if ((handle = _wfindfirst(path.c_str(), &fd)) == -1L)
	{
		// �����̳� ������ ã�� �� ���� ���
		_findclose(handle);
		return;
	}
	do /* ���� Ž�� �ݺ�*/
	{
		/* ���� ��ü ���� Ȯ�� (file or Dir) */
		checkDirFile = IsFileOrDir(fd);
		if (0 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			SearchingDir(_fdList, (defaultPath + L"/" + fd.name).c_str());
		}
		else if (1 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			std::wstring fileName = fd.name;
			std::wstring fileFormat = fileName.substr(fileName.find_last_of(L".") + 1);
			std::wstring temp = defaultPath + L"/" + fileName;
			
			_fdList->emplace_back(temp.c_str());
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}

void CFileIO::SearchingFile(std::wstring* _fp, const TCHAR* _path, const std::wstring _name, const TCHAR* _extention)
{
	std::wstring defaultPath = _path;
	std::wstring path = defaultPath + L"/*.*";
	std::wstring extention = _extention;
	_int checkDirFile = 0;
	struct _wfinddata_t fd;
	intptr_t handle;

	/* for. fd ����ü �ʱ�ȭ */
	if ((handle = _wfindfirst(path.c_str(), &fd)) == -1L)
	{
		// �����̳� ������ ã�� �� ���� ���
		_findclose(handle);
		*_fp = L"";
		return;
	}
	do /* ���� Ž�� �ݺ�*/
	{
		/* ���� ��ü ���� Ȯ�� (file or Dir) */
		checkDirFile = IsFileOrDir(fd);
		if (0 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			SearchingFile(_fp, (defaultPath + L"/" + fd.name).c_str(), _name, extention.c_str());
		}
		else if (1 == checkDirFile && '.' != fd.name[0])
		{
			/*������ ���*/
			std::wstring fileName = fd.name;
			size_t targetNum = fileName.find_last_of(L".");
			std::wstring fileNameOnly = fileName.substr(0, targetNum);
			std::wstring fileFormat = fileName.substr(targetNum + 1);

			if (fileNameOnly == _name)
			{
				if (fileFormat == _extention)
				{
					*_fp = defaultPath + L"/" + fileName;
					return;
				}
			}
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}

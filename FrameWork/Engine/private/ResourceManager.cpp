#include "ResourceManager.h"
#include "FileIO.h"

CResourceManager::CResourceManager()
{
}

HRESULT CResourceManager::NativeConstruct(_bool bAllLoad, const std::wstring& EngineResourcePath, const std::wstring& ClientResourcePath)
{
	m_engineResourcePath = EngineResourcePath + L"/";
	m_clientResourcePath = ClientResourcePath + L"/";

	LoadAllTextures(EngineResourcePath, true);

	if (!bAllLoad)
		return S_OK;

	LoadAllTextures(m_clientResourcePath, true);

	return S_OK;
}

_int CResourceManager::Tick(_double dTimeDelta)
{
	return _int();
}

void CResourceManager::ReleaseSceneResource(void)
{
}

void CResourceManager::LoadAllTextures(const std::wstring& _directoryPath, _bool _bStatic)
{
	LoadTextures(_directoryPath, L"dds", _bStatic);
}

void CResourceManager::LoadTextures(const std::wstring& _path, const TCHAR* _extention, _bool _bStatic)
{
	list<std::wstring> filePaths;

	CFileIO::SearchingDir(&filePaths, _path.c_str(), _extention);

	for (auto& file : filePaths)
	{
		std::wstring filePath = file;
		std::wstring fileName = filePath.substr(filePath.find_last_of(L"/" + 1));
		size_t targetNum = fileName.find_last_of(L".");
		fileName = fileName.substr(0, targetNum);

		targetNum = fileName.find_last_of(L"_");
		if (-1 != targetNum)
		{
			_bool isSprite = false;
			if (targetNum + 4 < fileName.length())
			{
				std::wstring sprite = fileName.substr(targetNum + 1, 3);
				if (sprite == L"spr")
				{
					std::wstring fileIndex = fileName.substr(targetNum + 4, fileName.length());

					_int texIndex = 0;
					if (IsDigitString(&texIndex, fileIndex))
					{
						isSprite = true;

						fileName = fileName.substr(0, targetNum);

						
					}
				}
			}
		}
	}
}

void CResourceManager::LoadResourcesBySceneFoloderName(const std::wstring& _sceneName, _bool _bStatic)
{
}

std::wstring CResourceManager::GetClientResourceDirectoryPath(void)
{
	return std::wstring();
}

std::wstring CResourceManager::GetEngineResourceDirectoryPath(void)
{
	return std::wstring();
}

void CResourceManager::Free()
{
}

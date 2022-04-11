#include "ResourceManager.h"
#include "Engine_Function.h"
#include "FileIO.h"
#include "UI_Texture.h"

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
	for (auto& resources : m_resourceContainers)
	{
		for (auto iter = resources.second.begin(); iter != resources.second.end();)
		{
			if ((*iter).second)
			{
				if ((*iter).second->m_bStatic)
				{
					++iter;
					continue;
				}
				auto resource = (*iter).second;
				iter = resources.second.erase(iter);
				resource->Free();
				Safe_Delete(resource);
			}
		}
	}
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
		wstring fileName = filePath.substr(filePath.find_last_of(L"/") + 1);
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
						UI_Texture* tex = (UI_Texture*)m_resourceContainers[typeid(UI_Texture).name()][fileName];
						if (!tex)
						{
							tex = new UI_Texture;
							tex->m_bStatic = _bStatic;
							tex->m_name = fileName;
							m_resourceContainers[typeid(UI_Texture).name()][fileName] = tex;
						}
						tex->m_filePath = filePath;
						tex->NativeConstruct(filePath.c_str());
					}
				}
			}
			if (!isSprite)
			{
				if (m_resourceContainers[typeid(UI_Texture).name()][fileName])
				{
					std::wstring msg;
					msg += L" Resource is already exist! \n";
					msg += L" Old File Path : " + m_resourceContainers[typeid(UI_Texture).name()][fileName]->m_filePath + L"." + _extention + L"\n";
					msg += L" New File Path : " + filePath + L"." + _extention;
					PRINT_LOG(L"Warrning", msg.c_str());
					continue;
				}
				UI_Texture* tex = new UI_Texture();
				assert("Fail to Create Resource!" && tex);
				tex->m_bStatic = _bStatic;
				tex->m_name = fileName;
				tex->m_filePath = filePath;
				tex->NativeConstruct(filePath.c_str());
				m_resourceContainers[typeid(UI_Texture).name()][fileName] = tex;
			}
		}
		else
		{
			if (m_resourceContainers[typeid(UI_Texture).name()][fileName])
			{
				std::wstring msg;
				msg += L" Resource is already exist! \n";
				msg += L" Old File Path : " + m_resourceContainers[typeid(UI_Texture).name()][fileName]->m_filePath + L"." + _extention + L"\n";
				msg += L" New File Path : " + filePath + L"." + _extention;
				PRINT_LOG(L"Warrning", msg.c_str());
				
				continue;
			}
			UI_Texture* tex = new UI_Texture();
			tex->m_bStatic = _bStatic;
			tex->m_name = fileName;
			tex->m_filePath = filePath;
			assert("Fail to Create Resource!" && tex);
			tex->NativeConstruct(filePath.c_str());
			m_resourceContainers[typeid(UI_Texture).name()][fileName] = tex;
		}
	}
}

void CResourceManager::LoadResourcesBySceneFoloderName(const std::wstring& _sceneName, _bool _bStatic)
{
	wstring resourceDirectory = m_clientResourcePath + L"/" + _sceneName;

	LoadAllTextures(resourceDirectory, _bStatic);
}

std::wstring CResourceManager::GetClientResourceDirectoryPath(void)
{
	return m_clientResourcePath;
}

std::wstring CResourceManager::GetEngineResourceDirectoryPath(void)
{
	return m_engineResourcePath;
}

void CResourceManager::Free()
{
	for (auto& resources : m_resourceContainers)
	{
		for (auto& resource : resources.second)
		{
			if (resource.second)
			{
				resource.second->Free();
				Safe_Delete(resource.second);
			}
		}
		resources.second.clear();
	}
	m_resourceContainers.clear();
}

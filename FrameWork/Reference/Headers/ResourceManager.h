#ifndef ResourceManager_h__
#define ResourceManager_h__

#include "SingleTon.h"
#include "IResource.h"
#include "Engine_Defines.h"
#include "Level_Manager.h"

BEGIN(Engine)

class CResourceManager final : public CSingleTon<CResourceManager>
{
	friend CSingleTon;

private:
	NO_COPY(CResourceManager);
	explicit CResourceManager();
	virtual ~CResourceManager() = default;

public:
	HRESULT NativeConstruct(_bool bAllLoad, const std::wstring& EngineResourcePath, const std::wstring& ClientResourcePath);
	_int	Tick(_double dTimeDelta);

public:
	void ReleaseSceneResource(void);
	template <typename TYPE, typename T>
	T* AddResource(const std::wstring& Name, _bool bStatic = false)
	{
		TYPE* resource = new T();
		resource->bStatic = bStatic;
		((IResource*)resource)->m_name = Name;
		resource->NativeConstruct(L"");
		m_resourceContainers[typeid(TYPE).name()][Name] = resource;

		return (T*)resource;
	}

private:
	template<typename TYPE, typename T>
	TYPE* GetResource(const std::wstring& Name, const TCHAR* Extention)
	{
		IResource* resource = m_resourceContainers[typeid(TYPE).name()][Name];
		if (nullptr == resource)
		{
			std::wstring targetFile;
			CFileIO::SearchingFile(&targetFile, m_clientResourcePath.c_str(), Name, Extention);

			if (L"" != targetFile)
			{
				std::wstring filePath = targetFile;
				std::wstring fileName = filePath.substr(filePath.find_last_of(L"/") + 1);
				size_t targetNum = fileName.find_last_of(L".");
				fileName = fileName.substr(0, targetNum);

				if (fileName == Name)
				{
					TYPE* resource = new T();
					resource->m_bStatic = false;
					assert("Faile to Create Resource!" && resource);
					((IResource*)resource)->m_name = fileName;
					resource->m_filePath = filePath;
					resource->NativeConstruct(filePath);
					m_resourceContainers[typeid(T).name()][fileName] = resource;

					std::wstring sceneLevel = CLevel_Manager::GetInstance()->getCurrentLevel();
					std::wstring message = L"Can not find resource! \nScene Level :";
					message += sceneLevel;
					message += L"\nResource Name : ";
					message += fileName;
					message += L"\nAlternate file path : ";
					message += filePath;

					PRINT_LOG("Warnning", (message).c_str());

					return (TYPE*)resource;
				}
			}
		}
		else
		{
			return (TYPE*)resource;
		}
		PRINT_LOG("Warnning", (L"Resource with that name '" + Name + L"' does not exist").c_str());
		return nullptr;
	}

public:
	template<typename T>
	T* GetResource(const std::wstring& _name)
	{
		IResource* resource = m_resourceContainers[typeid(T).name()][_name];

		if (nullptr == resource)
		{
			list<std::wstring> filePaths;
			CFileIO::SearchingDir(&filePaths, m_clientResourcePath.c_str());

			for (auto& file : filePaths)
			{
				std::wstring filePath = file;
				std::wstring fileName = filePath.substr(filePath.find_last_of(L"/" + 1));
				size_t targetNum = fileName.find_last_of(L".");
				fileName = fileName.substr(0, targetNum);

				if (fileName == _name)
				{
					T* resource = new T();
					resource->m_bStatic = false;
					assert("Fail to Create Resource!" && resource);
					((IResource*)resource)->m_name = fileName;
					resource->m_filePath = filePath;
					resource->NativeConstruct(filePath);
					m_resourceContainers[typeid(T).name()][fileName] = resource;

					std::wstring sceneName = CLevel_Manager()->GetInstance()->getCurrentLevel();
					std::wstring messgae = L"Can not find resource! \nScene Level : ";
					message += sceneName;
					message += L"\nResource Name : ";
					message += fileName;
					message += L"\nAlternate file path : ";
					message += filePath;

					PRINT_LOG("Warnning", (message).c_str());

					return (T*)resource;
				}
			}
		}
		else
		{
			return (T*)resource;
		}
		PRINT_LOG("Warnning", (L"Resource with that name '" + Name + L"' does not exist").c_str());
		return nullptr;
	}

	template<typename T>
	T* GetResourceIfExist(const std::wstring& _name)
	{
		const char* typeId = typeid(T).name();
		if (m_resourceContainers[typeId].find(_name) == m_resourceContainers[typeId].end())
			return nullptr;

		IResource* resource = m_resourceContainers[typeId][_name];
		return (T*)resource;
	}

private:
	std::unordered_map<string, std::unordered_map<std::wstring, IResource*>> m_resourceContainers;
	typedef std::unordered_map<string, std::unordered_map<std::wstring, IResource*>> RESOURCES;

private:
	std::wstring m_engineResourcePath;
	std::wstring m_clientResourcePath;

public:
	void LoadAllTextures(const std::wstring& _directoryPath, _bool _bStatic);
	void LoadTextures(const std::wstring& _path, const TCHAR* _extention, _bool _bStatic);
	void LoadResourcesBySceneFoloderName(const std::wstring& _sceneName, _bool _bStatic);

public:
	template <typename TYPE, typename T>
	void UpdateResource(const std::wstring _filePath, const std::wstring& _extention, _bool _bStatic = false)
	{
		list<std::wstring> filePaths;
		CFileIO::SearchingDir(&filePaths, _filePath.c_str(), _extention.c_str());

		for (auto& file : filePaths)
		{
			std::wstring filePath = file;
			std::wstring fileName = filePath.substr(filePath.find_last_of(L"/") + 1);
			size_t targetNum = fileName.find_last_of(L".");
			fileName = fileName.substr(0, targetNum);

			if (m_resourceContainers[typeid(TYPE).name()][fileName])
			{
				TYPE resource;
				resource.NativeConstruct(filePath);
				TYPE* existingResource = (TYPE*)m_resourceContainers[typeid(TYPE).name()][fileName];
				*existingResource = resource;
				existingResource->m_name = fileName;
			}
			else
			{
				TYPE* resource = new T();
				resource->m_bStatic = _bStatic;
				assert("Fail to Create Resource!" && resource);
				((IResource*)resource)->m_name = fileName;
				resource->m_filePath = filePath;
				resource->NativeConstruct(filePath);
				m_resourceContainers[typeid(TYPE).name()][fileName] = resource;

			}
		}
	}

	template <typename TYPE, typename T>
	void LoadResource(const std::wstring& _filePath, _bool _bStatic = false)
	{
		std::wstring filePath = _filePath;
		std::wstring fileName = filePath.substr(filePath.find_last_of(L"/" + 1));
		size_t targetNum = fileName.substr(L".");
		fileName = fileName.substr(0, targetNum);

		if (m_resourceContainers[typeid(TYPE).name()][fileName])
		{
			std::wstring message;
			message += L" Resource is already exist! \n";
			message += L" Old File Path : " + m_resourceContainers[typeid(TYPE).name()][fileName]->m_filePath + L"." + _extention + L"\n";
			message += L" New File Path : " + filePath + L"." + _extention;

			PRINT_LOG("Warnning", (message).c_str());
			continue;
		}
		TYPE* resource = new T();
		resource->m_bStatic = _bStatic;
		assert("Fail to Create Resource!" && resource);
		((IResource*)resource)->m_name = fileName;
		resource->m_filePath = filePath;

		resource->NativeConstruct(filePath);
		m_resourceContainers[typeid(TYPE).name()][fileName] = resource;
	}

public:
	std::wstring GetClientResourceDirectoryPath(void);
	std::wstring GetEngineResourceDirectoryPath(void);

public:
	virtual void Free() override;
};

END
#endif // ResourceManager_h__
#ifndef DataManager_h__
#define DataManager_h__

#include "SingleTon.h"
BEGIN(Client)

class CScriptableData;
class CDataManager final : public CSingleTon<CDataManager>
{
	friend CSingleTon;

private:
	NO_COPY(CDataManager);
	CDataManager(void);
	~CDataManager(void) = default;

public:
	HRESULT NativeConstruct(void);
	_int    Tick(void);

public:
	void AddScriptableData(std::wstring Key, CScriptableData* pData);

	CScriptableData* GetScriptableData(std::wstring Key);
	template <typename T>
	T* GetScriptableData(std::wstring key)
	{
		return static_cast<T*>(m_scriptableDatas[key]);
	}

private:
	std::unordered_map<std::wstring, CScriptableData*> m_scriptableDatas;

private:
	virtual void Free() override;
};

#define  ADD_DATA(Key, Data) CDataManager::GetInstance()->AddScriptableData(Key, Data)
#define  GET_DATA(type, key) GetScriptableData<type>(key)

END
#endif // DataManager_h__
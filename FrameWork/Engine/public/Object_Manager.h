#pragma once

#include "SingleTon.h"

/* ��ü(����)���� ��Ƽ� �����Ѵ�. */
/* ��ü(���纻)���� �������� ���� ���ؿ� ���� �����(CLayer)  �����Ѵ�.*/
/* �����ϰ��ִ� ��ü���� Tick�� ȣ�����ֳ�. */
/* �����ϰ��ִ� ��ü���� LateTick�� ȣ�����ֳ�. */

BEGIN(Engine)

class CObject_Manager final : public CSingleTon<CObject_Manager>
{
	friend CSingleTon;
private:
	NO_COPY(CObject_Manager);
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void* pArg);
	CGameObject* Clone_Gameobject(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
public:
	list<CGameObject*>* getObjectList(_uint iLevelIndex, const wstring& pLayerTag);
public:
	HRESULT Clear_Object_Manager(_uint iLevelIndex);
	void	Clear_Object_List(void);

private:
	unordered_map<wstring, class CGameObject*>			m_Prototype;
	typedef unordered_map<wstring, class CGameObject*>	PROTOTYPES;

	unordered_map<wstring, class CLayer*>*				m_pLayers = nullptr;
	typedef unordered_map<wstring, class CLayer*>			LAYERS;

private:
	CGameObject* Find_Prototype(const wstring& pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& pLayerTag);
private:
	_uint			m_iNumLevels = 0;
public:
	virtual void Free() override;
};

END
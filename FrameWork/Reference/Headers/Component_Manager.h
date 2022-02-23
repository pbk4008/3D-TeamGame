#pragma once

#include "SingleTon.h"

#include "Model.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_RectInstance.h"
#include "VIBuffer_PointInstance.h"

BEGIN(Engine)

class CComponent_Manager final : public CSingleTon<CComponent_Manager>
{
friend CSingleTon;
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;	
public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg);
	
private:	
	unordered_map<wstring, class CComponent*>*			m_pComponents = nullptr;
	typedef unordered_map<wstring, class CComponent*>		COMPONENTS;

	_uint			m_iNumLevels = 0;
private:
	CComponent* Find_Prototype_Component(_uint iLevelIndex, const wstring& pPrototypeTag);
public:
	virtual void Free() override;
};

END
#pragma once

#include "SingleTon.h"

#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Gizmo.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "Ray_Collider.h"
#include "Renderer.h"
#include "Transform.h"
//#include "Navigation.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Trapezium.h"
#include "VIBuffer_RectInstance.h"
#include "VIBuffer_PointInstance.h"
#include "VIBuffer_PointInstance_Explosion.h"
#include "Instancing_Mesh.h"
#include "VIBuffer_Plane.h"
#include "StateController.h"
#include "AnimationController.h"
#include "Animator.h"
#include "CharacterController.h"

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
public:
	HRESULT SetUpBaseComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void    Clear_Component(void);
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
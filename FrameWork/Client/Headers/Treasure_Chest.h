#ifndef Treasure_Chest_h__
#define Treasure_Chest_h__
#include "GameObject.h"

BEGIN(Client)

class CTreasure_Chest : public CGameObject
{
public:
	typedef struct tagEnvironmentDesc
	{
		wstring wstrInstaneTag = L"";
		CInstancing_Mesh::INSTANCINGMESHDESC tInstanceDesc;
	}ENVIRONMENTDESC;

private:
	explicit CTreasure_Chest(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CTreasure_Chest(const CTreasure_Chest& _rhs);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

public:
	void Raycast_FromMouse(const _double& _dDeltaTime);
	_bool Raycast(const _fvector& _svRayPos, const _fvector& _svRayDir, _float& _fOutDist, const _double& _dDeltaTime);

private:
	CModel* m_pModel = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CRay_Collider* m_pCollider = nullptr;

	_bool	  m_isPick = false;
	_float	  m_fHoldTime = 0.f;
	_float4x4 m_vecWorldMat;

public:
	static CTreasure_Chest* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
#endif // Treasure_Chest_h__
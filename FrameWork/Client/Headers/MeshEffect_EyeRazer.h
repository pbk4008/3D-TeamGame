#pragma once

#include "MeshEffect.h"



BEGIN(Client)

class CMeshEffect_EyeRazer : public CMeshEffect
{
private:
	explicit CMeshEffect_EyeRazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMeshEffect_EyeRazer(const CMeshEffect_EyeRazer& _rhs);
	virtual ~CMeshEffect_EyeRazer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void Set_EyeNode(CHierarchyNode* pNode) { m_pEyeNode = pNode; }
	void Set_RazerRotate(_bool check) { m_bRotate = check; }
private:
	HRESULT Ready_Components();

private:
	class CHierarchyNode* m_pEyeNode = nullptr;

	_float m_fAngle = 0.f;
	_bool m_bRotate = false;

public:
	static CMeshEffect_EyeRazer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr);
	virtual void Free() override;
};

END
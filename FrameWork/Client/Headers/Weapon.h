#pragma once

#include "GameObject.h"

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
public:
	enum class EType { None, Sword_1H, Sword_2H, Hammer_2H, Shield, Max};
protected:
	explicit CWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CWeapon(const CWeapon& _rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public:
	const wstring& Get_Name() const;
	const EType Get_Type() const;

	void Set_Owner(CGameObject* _pOwner);
	void Set_OwnerPivotMatrix(const _fmatrix& _smatPivot);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);
	void Set_FixedBone(CHierarchyNode* _pFixedBone);

protected:
	CTransform* m_pLocalTransform = nullptr;
	CModel* m_pModel = nullptr;
	_matrix m_smatOwnerPivot = XMMatrixIdentity();

	CGameObject* m_pOwner = nullptr;
	CHierarchyNode* m_pFixedBone = nullptr;

	_bool m_isEquip = false;
	EType m_eType = EType::Max;

	wstring m_wstrName = L"";

public:
	virtual CGameObject* Clone(void* _pArg = nullptr) PURE;
	virtual void Free() override;
};

END
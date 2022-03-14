#include "pch.h"
#include "Weapon.h"


CWeapon::CWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CWeapon::CWeapon(const CWeapon& _rhs)
	: CGameObject(_rhs)
	, m_eType(_rhs.m_eType)
	, m_wstrName(_rhs.m_wstrName)
{
}

HRESULT CWeapon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	return S_OK;
}

_int CWeapon::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	return _int();
}

_int CWeapon::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;
	
	return _int();
}

HRESULT CWeapon::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

const wstring& CWeapon::Get_Name() const
{
	return m_wstrName;
}

const CWeapon::EType CWeapon::Get_Type() const
{
	return m_eType;
}

void CWeapon::Set_Owner(CGameObject* _pOwner)
{
	m_pOwner = _pOwner;
}

void CWeapon::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	_float fRadian = XMConvertToRadians(180.f);
	m_smatOwnerPivot = XMMatrixRotationRollPitchYaw(fRadian, fRadian, fRadian) * _smatPivot;
}

void CWeapon::Set_Equip(const _bool _isEquip, void* _pArg)
{
	m_isEquip = _isEquip;
	//if (_pArg)
	//	m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);
}

void CWeapon::Set_FixedBone(CHierarchyNode* _pFixedBone)
{
	m_pFixedBone = _pFixedBone;
}

void CWeapon::Free()
{
	Safe_Release(m_pLocalTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}

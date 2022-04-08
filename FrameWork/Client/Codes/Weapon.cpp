#include "pch.h"
#include "Weapon.h"
#include "SwordTrail.h"

CWeapon::CWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
	m_bActive = true;
	ZeroMemory(&m_smatOwnerPivot, sizeof(_float4x4));
}

CWeapon::CWeapon(const CWeapon& _rhs)
	: CGameObject(_rhs)
	, m_eType(_rhs.m_eType)
	, m_wstrName(_rhs.m_wstrName)
	, m_smatOwnerPivot(_rhs.m_smatOwnerPivot)
{
	m_bActive = _rhs.m_bActive;
}

HRESULT CWeapon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

	//list<CGameObject*>* listobj = g_pGameInstance->getObjectList(_iSceneID, L"Layer_SordTrail");
	//m_pTrail = static_cast<CSwordTrail*>(listobj->front());

	//if (m_pTrail == nullptr) return E_FAIL;

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

CActor* CWeapon::Get_Owner() const
{
	return m_pOwner;
}

const _float CWeapon::Get_Damage() const
{
	return m_fDamage;
}

void CWeapon::Set_Owner(CActor* _pOwner)
{
	m_pOwner = _pOwner;
}

void CWeapon::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	_float fRadian = XMConvertToRadians(180.f);
	_matrix matResut = XMMatrixRotationRollPitchYaw(fRadian, fRadian, fRadian) * _smatPivot;
	XMStoreFloat4x4(&m_smatOwnerPivot, matResut);
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

void CWeapon::Set_IsAttack(const _bool _isAttack)
{
	m_isAttack = _isAttack;
}

void CWeapon::Set_IsTrail(const _bool _isTrail)
{
	if (_isTrail != m_isTrail)
		m_isTrail = _isTrail;
}

const _bool CWeapon::IsAttack() const
{
	return m_isAttack;
}

const _bool CWeapon::IsTrail() const
{
	return m_isTrail;
}

void CWeapon::RangeAttack()
{
}

_fmatrix CWeapon::Remove_Scale(_fmatrix matTransform)
{
	//Right벡터 Nomalize해서 구하기
	_vector vRight = XMVector3Normalize(matTransform.r[0]);
	//Up벡터 Nomalize해서 구하기
	_vector vUP = XMVector3Normalize(matTransform.r[1]);
	//Look벡터 Nomalize해서 구하기
	_vector vLook = XMVector3Normalize(matTransform.r[2]);
	
	//결과 벡터
	_matrix matResult = XMMatrixIdentity();

	//결과 벡터에 right넣기
	matResult.r[0] = vRight;
	//결과 벡터에 Up넣기
	matResult.r[1] = vUP;
	//결과 벡터에 Look넣기
	matResult.r[2] = vLook;
	//결과 벡터에 가져온 Pos넣기
	matResult.r[3] = matTransform.r[3];

	return matResult;
}

void CWeapon::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}

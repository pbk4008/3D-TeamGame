#include "pch.h"
#include "Weapon.h"
#include "SwordTrail.h"

CWeapon::CWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
	m_bActive = true;
	ZeroMemory(&m_matOwnerPivot, sizeof(_float4x4));
}

CWeapon::CWeapon(const CWeapon& _rhs)
	: CGameObject(_rhs)
	, m_eType(_rhs.m_eType)
	, m_wstrName(_rhs.m_wstrName)
	, m_matOwnerPivot(_rhs.m_matOwnerPivot)
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

	m_lightdesc = g_pGameInstance->Get_LightDesc(0);
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

HRESULT CWeapon::BindConstantBuffer(const wstring& camTag,SCB* consbuffer)
{
	if(m_pTransform == nullptr)
		MSGBOX("Failed To Apply Weapon Transform nullptr");

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(camTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(camTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");

	if (consbuffer)
	{
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_Metalic", &consbuffer->metalic, sizeof(_float)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_Roughness", &consbuffer->roughness, sizeof(_float)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_AO", &consbuffer->ao, sizeof(_float)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_color", &consbuffer->color, sizeof(_float4)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &consbuffer->empower, sizeof(_float)))) MSGBOX("Failed To Apply Weapon ConstantBuffer");
	}

	return S_OK;
}

HRESULT CWeapon::BindLightBuffer()
{
	_matrix view, porj;
	_float3 lightpos = m_lightdesc->vPosition;
	view = XMMatrixTranspose(m_lightdesc->mLightView);
	porj = XMMatrixTranspose(m_lightdesc->mLightProj);

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightView", &view, sizeof(_matrix)))) MSGBOX("Failed To Apply Weapon LightConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightProj", &porj, sizeof(_matrix)))) MSGBOX("Failed To Apply Weapon LightConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightPos", &lightpos, sizeof(_float3)))) MSGBOX("Failed To Apply Weapon LightConstantBuffer");

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

const _float3& CWeapon::Get_EndPos() const
{
	return m_vEndPos;
}

void CWeapon::Set_Owner(CActor* _pOwner)
{
	m_pOwner = _pOwner;
}

void CWeapon::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	_float fRadian = XMConvertToRadians(180.f);
	_matrix matResut = XMMatrixRotationRollPitchYaw(fRadian, fRadian, fRadian) * _smatPivot;
	XMStoreFloat4x4(&m_matOwnerPivot, matResut);
}

void CWeapon::Set_Equip(const _bool _isEquip, void* _pArg)
{
	m_isEquip = _isEquip;
	if(nullptr != _pArg)
		Set_Owner((CActor*)_pArg);
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

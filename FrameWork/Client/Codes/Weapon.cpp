#include "pch.h"
#include "Weapon.h"

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

	m_dissolveTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_dissolveTex->Change_Texture(L"DissovleBase"))) MSGBOX("Failed to Change Texture DissovleTex");

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

HRESULT CWeapon::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 2);

	return S_OK;
}

HRESULT CWeapon::Render_Velocity()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));

	MOTIONBLUR motion;
	ZeroMemory(&motion, sizeof(MOTIONBLUR));
	// velocity desc
	_float4x4 rot;
	XMStoreFloat4x4(&rot, m_pTransform->Get_WorldMatrix()
		* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW)
		* g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	rot._11 = 1.0f; rot._22 = 1.0f; rot._33 = 1.0f;
	rot._41 = 0.0f; rot._42 = 0.0f; rot._43 = 0.0f;
	motion.RotationMat = rot;
	_matrix prewvp = g_pGameInstance->GetPreViewProtj(m_PreWroldMat);
	XMStoreFloat4x4(&motion.preWorldViewPorjMat, prewvp);
	//----------------------------------------------------

	CWeapon::BindConstantBuffer(wstrCamTag, &desc, &rimdesc, &motion);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		if (FAILED(m_pModel->Render(i, 3))) MSGBOX("Fialed To Rendering Silvermane");

	m_timer += g_fDeltaTime;
	if (m_timer >= 0.05f)
	{
		m_PreWroldMat = m_pTransform->Get_WorldMatrix();
		m_timer = 0.f;
	}

	return S_OK;
}

HRESULT CWeapon::BindConstantBuffer(const wstring& camTag,SCB* consbuffer, RIM* rimlightbuffer, MOTIONBLUR* motionbuffer)
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

	if (rimlightbuffer)
	{
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimlightcheck", &rimlightbuffer->rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimintensity", &rimlightbuffer->rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimcolor", &rimlightbuffer->rimcol, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_camdir", &rimlightbuffer->camdir, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	}

	if (motionbuffer)
	{
		_matrix rot, wvp;
		rot = XMMatrixTranspose(XMLoadFloat4x4(&motionbuffer->RotationMat));
		wvp = XMMatrixTranspose(XMLoadFloat4x4(&motionbuffer->preWorldViewPorjMat));

		if (FAILED(m_pModel->SetUp_ValueOnShader("g_RotationMat", &rot, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_PreWorldViewProj", &wvp, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
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

HRESULT CWeapon::WeaponToAppear()
{
	m_lifetime -= (g_fDeltaTime);
	if (m_lifetime <= 0.f)
	{
		m_lifetime = 0.f;
		m_dissolvepass = 1;
		m_bdissolveappear = false;
	}
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_dissolvetime", &m_lifetime, sizeof(_float)))) MSGBOX("Failed to Apply dissolvetime");
	if (FAILED(m_pModel->SetUp_TextureOnShader("g_DissolveTex", m_dissolveTex, 0))) MSGBOX("Failed to Apply dissolveTex");

	return S_OK;
}

HRESULT CWeapon::WeaponToDisAppear()
{
	m_dissolvepass = 1;

	m_lifetime += (g_fDeltaTime);
	if (m_lifetime >= 1.f)
	{
		m_lifetime = 1.f;
		m_bdissolvedisappear = false;
	}
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_dissolvetime", &m_lifetime, sizeof(_float)))) MSGBOX("Failed to Apply dissolvetime");
	if (FAILED(m_pModel->SetUp_TextureOnShader("g_DissolveTex", m_dissolveTex, 0))) MSGBOX("Failed to Apply dissolveTex");


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

void CWeapon::RimlightCheck(_bool check)
{
	m_rimcheck = check;
	if (check == false)
		m_rimintensity = 5.f;
}

void CWeapon::SetRimIntensity(_float time)
{
	m_rimintensity += time * 1.3f;
	
	if (m_rimintensity <= 2.0f)
		m_rimintensity = 2.0f;
}

void CWeapon::RangeAttack()
{
}

_fmatrix CWeapon::Remove_Scale(_fmatrix matTransform)
{
	//Right vector to Normalize
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
	Safe_Release(m_dissolveTex);
	Safe_Release(m_pLocalTransform);

	__super::Free();
}

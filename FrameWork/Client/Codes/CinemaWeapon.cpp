#include "pch.h"
#include"CinemaWeapon.h"
#include "Material.h"
#include "HierarchyNode.h"

CCinemaWeapon::CCinemaWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)	
{
}

CCinemaWeapon::CCinemaWeapon(const CCinemaWeapon& rhs)
	: CWeapon(rhs)
{
}

HRESULT CCinemaWeapon::NativeConstruct_Prototype()
{
	if (FAILED(CWeapon::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCinemaWeapon::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CWeapon::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_iType = (*(_uint*)_pArg);
	if(FAILED(Ready_Model()))
		return E_FAIL;

	return S_OK;
}

_int CCinemaWeapon::Tick(_double _dDeltaTime)
{
	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);
	return _int();
}

_int CCinemaWeapon::LateTick(_double _dDeltaTime)
{
	if (m_pRenderer == nullptr)
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CCinemaWeapon::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = _float4(0.7529f, 0.7529f, 0.7529f, 1.f);
	desc.empower = 0.7f;

	CWeapon::BindConstantBuffer(wstrCamTag, &desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

void CCinemaWeapon::ResetBone()
{
	m_pLocalTransform->Set_WorldMatrix(XMMatrixIdentity());
	m_pFixedBone = nullptr;
	XMStoreFloat4x4(&m_matOwnerPivot, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matOwerWorld, XMMatrixIdentity());
}

HRESULT CCinemaWeapon::Ready_Model()
{
	HRESULT hr = S_OK;
	switch (m_iType)
	{
	case 0:
		hr = CGameObject::SetUp_Components(m_iSceneID, L"Model_GrayHawkWeapon", L"Weapon_Model", (CComponent**)&m_pModel);
		hr=Ready_GrayHwakMaterial();
		break;
	case 1:
		hr = CGameObject::SetUp_Components(m_iSceneID, L"Model_PoenixWeapon", L"Weapon_Model", (CComponent**)&m_pModel);
		hr = Ready_PoenixMaterial();
		break;
	}

	if (FAILED(hr))
	{
		MSGBOX("Fail  Cinema Weapon");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCinemaWeapon::Ready_PoenixMaterial()
{
	CMaterial* pMtrl = g_pGameInstance->Get_Material(L"Mtrl_PoenixWeapon");
	if (!pMtrl)
	{
		CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Phoenix/T_1H_sword_Phoenix_D.dds", 1);
		pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_PoenixWeapon", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
		pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Phoenix/T_1H_sword_Phoenix_N.dds", 1);
		pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Phoenix/T_1H_sword_Phoenix_MRA.dds", 1);
		pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Phoenix/T_1H_sword_Phoenix_CEO.dds", 1);
		pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);

		g_pGameInstance->Add_Material(L"Mtrl_PoenixWeapon", pMtrl);

		return S_OK;
	}
	m_pModel->Add_Material(pMtrl, 0);

	return S_OK;
}

HRESULT CCinemaWeapon::Ready_GrayHwakMaterial()
{
	CMaterial* pMtrl = g_pGameInstance->Get_Material(L"Mtrl_GrayHwakWeapon");
	if (!pMtrl)
	{
		CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/GrayHwak/T_2H_Polearm_DawnsEye_D.dds", 1);
		pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_GrayHwakWeapon", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
		pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/GrayHwak/T_2H_Polearm_DawnsEye_N.dds", 1);
		pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/GrayHwak/T_2H_Polearm_DawnsEye_MRA.dds", 1);
		pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/GrayHwak/T_2H_Polearm_DawnsEye_CEO.dds", 1);
		pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);

		g_pGameInstance->Add_Material(L"Mtrl_GrayHwakWeapon", pMtrl);

		return S_OK;
	}
	m_pModel->Add_Material(pMtrl, 0);

	return S_OK;
}

_int CCinemaWeapon::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		//뼈노드가 가지고 있는 Combine행렬 가져옴
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		//무기 가지고 있는 객체의 피벗 곱해줌
		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);
		//무기 로컬 트랜스 폼 갱신
		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}
	return _int();
}

_int CCinemaWeapon::Attach_Owner(const _double& _dDeltaTime)
{
	_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
	_matrix smatOwerWorld = XMLoadFloat4x4(&m_matOwerWorld);
	m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	return 0;
}

void CCinemaWeapon::set_OwerMatrix(_fmatrix matWorld)
{
	XMStoreFloat4x4(&m_matOwerWorld, matWorld);
}


CCinemaWeapon* CCinemaWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCinemaWeapon* pInstance = new CCinemaWeapon(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCinemaWeapon Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCinemaWeapon::Clone(const _uint iSceneID, void* pArg)
{
	CCinemaWeapon* pInstance = new CCinemaWeapon(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CCinemaWeapon Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinemaWeapon::Free()
{
	CWeapon::Free();
}

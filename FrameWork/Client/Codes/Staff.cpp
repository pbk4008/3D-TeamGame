#include "pch.h"
#include "Staff.h"

#include "HierarchyNode.h"
#include "Monster_Bastion_Healer.h"
#include "Material.h"
#include "StateController.h"

CStaff::CStaff(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CStaff::CStaff(const CStaff& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CStaff::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Staff;
	m_wstrName = L"Staff";

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Staff", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Staff/T_bastion_staff_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Staff/T_bastion_staff_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Staff/T_bastion_staff_OMER.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_OMER, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Staff", pMtrl);

	return S_OK;
}

HRESULT CStaff::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	return S_OK;
}

_int CStaff::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	return _int();
}

_int CStaff::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		return 0;
	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CStaff::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = _float4(0.588f, 0.294f, 0.f, 1.f);
	desc.empower = 0.3f;

	CWeapon::BindConstantBuffer(wstrCamTag,&desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CStaff::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

HRESULT CStaff::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Staff", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Staff"), 0);

	return S_OK;
}

_int CStaff::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		if (!m_isEquip)
		{
			smatWorld = m_smatPivot * smatWorld;
		}

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CStaff::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CStaff* CStaff::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStaff* pInstance = new CStaff(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CStaff Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStaff::Clone(const _uint _iSceneID, void* _pArg)
{
	CStaff* pInstance = new CStaff(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CStaff Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStaff::Free()
{
	CWeapon::Free();
}

#include "pch.h"
#include "Stargazer.h"
#include "HierarchyNode.h"
#include "Monster_Bastion_Sword.h"
#include "Material.h"

CStargazer::CStargazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}
CStargazer::CStargazer(const CStargazer& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CStargazer::NativeConstruct_Prototype()
{
	if (FAILED(CWeapon::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Monster_Sword1H;
	m_iObectTag = (_uint)GAMEOBJECT::WEAPON;

	return S_OK;
}

HRESULT CStargazer::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CWeapon::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CStargazer::Tick(_double _dDeltaTime)
{
	//����� ������Ʈ
	Attach_FixedBone(_dDeltaTime);
	//���� ������ �ִ� ��ü�� ���� ������Ʈ
	Attach_Owner(_dDeltaTime);

	return _int();
}

_int CStargazer::LateTick(_double _dDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

	return _int();
}

HRESULT CStargazer::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	//�𵨿� ����Matrix ������
	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	//�𵨿�  ��Matrix ������
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	//�𵨿� ��������Matrix ������
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	
	//��� �޽������̳ʸ� ���鼭 ������
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);


	return S_OK;
}

void CStargazer::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	XMStoreFloat4x4(&m_smatOwnerPivot, _smatPivot);
}

HRESULT CStargazer::SetUp_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_Stargazer", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	if (FAILED(SetUp_Material()))
		return E_FAIL;

	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	//���� Ʈ������ ����
	m_pTransform->Set_TransformDesc(tDesc);
	//���� Ʈ������ ����
	m_pLocalTransform->Set_TransformDesc(tDesc);
	
	return S_OK;
}

_int CStargazer::Attach_FixedBone(const _double& _dDeltaTime)
{
	//�� ��尡 ������
	if (m_pFixedBone)
	{
		//����尡 ������ �ִ� Combine��� ������
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		//���� ������ �ִ� ��ü�� �ǹ� ������
		smatWorld *= XMLoadFloat4x4(&m_smatOwnerPivot);
		//���� ���� Ʈ���� �� ����
		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}
	return _int();
}

_int CStargazer::Attach_Owner(const _double& _dDeltaTime)
{
	//���� ������ �ִ� ��ü�� ������
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CMonster_Bastion_Sword*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}
	return _int();
}

HRESULT CStargazer::SetUp_Material()
{
	if (!m_pModel)
		return E_FAIL;

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_D.dds", 1);
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Stargazer", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);

	g_pGameInstance->Add_Material(L"Mtrl_Stargazer", pMtrl);

	m_pModel->Add_Material(pMtrl, 0);

	return S_OK;
}


CStargazer* CStargazer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStargazer* pInstance = new CStargazer(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CStargazer Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStargazer::Clone(const _uint _iSceneID, void* pArg)
{
	CStargazer* pInstance = new CStargazer(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CStargazer Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStargazer::Free()
{
	CWeapon::Free();
}

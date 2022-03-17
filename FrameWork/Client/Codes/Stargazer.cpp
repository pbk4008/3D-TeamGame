#include "pch.h"
#include "Stargazer.h"
#include "HierarchyNode.h"
#include "Monster_Bastion_Sword.h"

CStargazer::CStargazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
	ZeroMemory(&m_matTransform, sizeof(_float4x4));
}
CStargazer::CStargazer(const CStargazer& _rhs)
	: CWeapon(_rhs)
	, m_matTransform(_rhs.m_matTransform)
{
}

HRESULT CStargazer::NativeConstruct_Prototype()
{
	if (FAILED(CWeapon::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Sword_1H;
	if(FAILED(Set_Transform()))
		return E_FAIL;
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
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

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

HRESULT CStargazer::SetUp_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_Stargazer", L"Model", (CComponent**)&m_pModel)))
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
		smatWorld *= m_smatOwnerPivot;
	
		smatWorld *= XMLoadFloat4x4(&m_matTransform);
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
		//���� Ʈ������ ������
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		//���� ������ �ִ� ��ü�� Ʈ������ ������
		CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
		if (!pTransform)
			return -1;
		//���� ������ �ִ� ��ü�� ����Matrix������
		_matrix smatOwerWorld = pTransform->Get_WorldMatrix();
		//���� ������ �ִ� ��ü�� ����Matrix�� ũ�� ����
		smatOwerWorld = Remove_Scale(smatOwerWorld);
		//���� Ʈ�������� ���� ������ �ִ� ��ü�� ���� Ʈ�������� ���� ���� �־���
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}
	return _int();
}

HRESULT CStargazer::Set_Transform()
{
	//_matrix matPos = XMMatrixTranslation(-0.41f, 0.f, 0.7f);
	//_matrix matRotate = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));


	//XMStoreFloat4x4(&m_matTransform, matRotate*matPos);

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

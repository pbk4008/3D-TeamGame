#include "pch.h"
#include "Stargazer.h"
#include "HierarchyNode.h"
#include "Monster_Bastion_Sword.h"

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

	m_eType = EType::Sword_1H;
	return S_OK;
}

HRESULT CStargazer::NativeConstruct(void* _pArg)
{
	if (FAILED(CWeapon::NativeConstruct(_pArg)))
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

void CStargazer::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	XMStoreFloat4x4(&m_smatOwnerPivot, _smatPivot);
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
		_matrix matResult = XMMatrixIdentity();
		_matrix matRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(230), XMConvertToRadians(30), XMConvertToRadians(0));
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		//���� ������ �ִ� ��ü�� �ǹ� ������
		_matrix matPivot = XMLoadFloat4x4(&m_smatOwnerPivot);
		//smatWorld *= XMLoadFloat4x4(&m_matTransform);
		//���� ���� Ʈ���� �� ����

		matResult = matRotate * smatWorld * matPivot;

		m_pLocalTransform->Set_WorldMatrix(matResult);
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

		_matrix matResult = smatWorld * smatOwerWorld;
		//���� Ʈ�������� ���� ������ �ִ� ��ü�� ���� Ʈ�������� ���� ���� �־���
		m_pTransform->Set_WorldMatrix(matResult);
	}
	return _int();
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

CGameObject* CStargazer::Clone(void* pArg)
{
	CStargazer* pInstance = new CStargazer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
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

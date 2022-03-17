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
	//뼈행렬 업데이트
	Attach_FixedBone(_dDeltaTime);
	//무기 가지고 있는 객체의 월드 업데이트
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

	//모델에 월드Matrix 던져줌
	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	//모델에  뷰Matrix 던져줌
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	//모델에 프로젝션Matrix 던져줌
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	
	//모든 메쉬컨테이너를 돌면서 랜더함
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

	//메인 트랜스폼 셋팅
	m_pTransform->Set_TransformDesc(tDesc);
	//로컬 트랜스폼 셋팅
	m_pLocalTransform->Set_TransformDesc(tDesc);
	
	return S_OK;
}

_int CStargazer::Attach_FixedBone(const _double& _dDeltaTime)
{
	//뼈 노드가 있을때
	if (m_pFixedBone)
	{
		//뼈노드가 가지고 있는 Combine행렬 가져옴
		_matrix matResult = XMMatrixIdentity();
		_matrix matRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(230), XMConvertToRadians(30), XMConvertToRadians(0));
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		//무기 가지고 있는 객체의 피벗 곱해줌
		_matrix matPivot = XMLoadFloat4x4(&m_smatOwnerPivot);
		//smatWorld *= XMLoadFloat4x4(&m_matTransform);
		//무기 로컬 트랜스 폼 갱신

		matResult = matRotate * smatWorld * matPivot;

		m_pLocalTransform->Set_WorldMatrix(matResult);
	}
	return _int();
}

_int CStargazer::Attach_Owner(const _double& _dDeltaTime)
{
	//무기 가지고 있는 객체가 있을때
	if (m_pOwner)
	{
		//로컬 트랜스폼 가져옴
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		//무기 가지고 있는 객체의 트랜스폼 가져옴
		CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
		if (!pTransform)
			return -1;
		//무기 가지고 있는 객체의 월드Matrix가져옴
		_matrix smatOwerWorld = pTransform->Get_WorldMatrix();
		//무기 가지고 있는 객체의 월드Matrix중 크기 삭제
		smatOwerWorld = Remove_Scale(smatOwerWorld);

		_matrix matResult = smatWorld * smatOwerWorld;
		//메인 트랜스폼에 무기 가지고 있는 객체와 로컬 트랜스폼을 곱한 것을 넣어줌
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

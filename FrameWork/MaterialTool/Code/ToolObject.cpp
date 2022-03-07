#include "pch.h"
#include "ToolObject.h"
#include "HierarchyNode.h"

CToolObject::CToolObject() 
	: m_eType(TYPE::TYPE_END)
	, m_pModel(nullptr)
	, m_bClicked(false)
{
	ZeroMemory(&m_vRotateValue, sizeof(_float3));
}

CToolObject::CToolObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) : CGameObject(pDevice, pDeviceContext)
, m_eType(TYPE::TYPE_END)
, m_pModel(nullptr)
, m_bClicked(false)
{
	ZeroMemory(&m_vRotateValue, sizeof(_float3));
}

CToolObject::CToolObject(const CToolObject& rhs)
	: CGameObject(rhs)
	, m_eType(rhs.m_eType)
	, m_pModel(rhs.m_pModel)
	, m_vRotateValue(rhs.m_vRotateValue)
	, m_bClicked(rhs.m_bClicked)
	, m_iMeshNum(rhs.m_iMeshNum)
	, m_vecBone(rhs.m_vecBone)
{
	Safe_AddRef(m_pModel);
	m_vecCollider.reserve(rhs.m_vecCollider.size());

	for (auto& pCollider : rhs.m_vecCollider)
	{
		COLLIDER* newCollider = new COLLIDER;
		newCollider->pCollider = static_cast<CCollider*>(pCollider->pCollider->Clone(nullptr));
		newCollider->bBone = pCollider->bBone;
		m_vecCollider.emplace_back(newCollider);
	}

	for (auto& pBone : m_vecBone)
		Safe_AddRef(pBone);
}

HRESULT CToolObject::Init_GameObject(COMINDEX* pIndex)
{
	if (FAILED(CGameObject::Init_GameObject(pIndex)))
		return E_FAIL;

	if (FAILED(Add_Component<CRenderer>((_uint)TOOLSCENEID::TOOL_PROTO, COMPONENTID::RENDERER, m_pComponentIndex[(_uint)COMPONENTID::RENDERER].iStartIndex, &m_pRenderer)))
		return E_FAIL;
	if (FAILED(Add_Component<CTransform>((_uint)TOOLSCENEID::TOOL_PROTO, COMPONENTID::TRANSFORM, m_pComponentIndex[(_uint)COMPONENTID::RENDERER].iStartIndex, &m_pTransform)))
		return E_FAIL;
	if (FAILED(Add_Component<CModel>((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::MODEL, m_pComponentIndex[(_uint)COMPONENTID::MODEL].iStartIndex, &m_pModel)))
		return E_FAIL;

	CCollider* tmpCollider = nullptr;
	if (FAILED(Add_Component<CCollider>((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::COLLIDER, m_pComponentIndex[(_uint)COMPONENTID::COLLIDER].iStartIndex, &tmpCollider)))
		return E_FAIL;

	COLLIDER* newCollider = new COLLIDER;
	newCollider->pCollider = tmpCollider;
	newCollider->bBone = true;
	m_vecCollider.emplace_back(newCollider);

	setActive(true);

	return S_OK;
}

HRESULT CToolObject::Init_Clone(void* pArg)
{
	if (FAILED(CGameObject::Init_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CToolObject::Update_GameObject(_float fDeltaTime)
{
	if (0 > CGameObject::Update_GameObject(fDeltaTime))
		return -1;
	
	if (m_pModel->getModelType() == CModel::TYPE::TYPE_ANIM)
		m_pModel->Update_CombinedTransformationMatrix((_double)fDeltaTime, true);

	_uint iSize = (_uint)m_vecCollider.size();
	for (_uint i = 0; i < iSize; i++)
	{
		if(m_vecCollider[i]->bBone)
			m_vecCollider[i]->pCollider->Update(m_pTransform->getWorldMatrix());
		else
			m_vecCollider[i]->pCollider->Update(Calcul_BonMatrix(m_vecBone[i-1]));
	}
	m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CToolObject::Render_GameObject()
{
	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;


	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	_matrix matWorld = XMMatrixTranspose(m_pTransform->getWorldMatrix());
	_matrix matView = XMMatrixTranspose(pInstance->GetTransform(L"MainCamera",TRANSMATRIXSTATE::D3DT_VIEW));
	_matrix matProj = XMMatrixTranspose(pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_PROJ));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_float4x4));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_float4x4));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_float4x4));

	if (FAILED(m_pModel->Bind_Buffers()))
		return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		if(m_pModel->getModelType() == CModel::TYPE::TYPE_ANIM)
			m_pModel->Render(i, 1);
		else
			m_pModel->Render(i, 0);
	}

	for (auto& pCollider : m_vecCollider)
	{
		pCollider->pCollider->setUsingTool();
		pCollider->pCollider->Render();
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CToolObject::Add_Collider(CCollider* pCollider, _bool bBoneCheck)
{
	COLLIDER* newCollider = new COLLIDER;

	newCollider->pCollider = pCollider;
	newCollider->bBone = bBoneCheck;

	m_vecCollider.emplace_back(newCollider);
}

CToolObject* CToolObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex)
{
	CToolObject* pInstance = new CToolObject(pDevice, pDeviceContext);
	if (FAILED(pInstance->Init_GameObject(pIndex)))
	{
		MSG_BOX(L"CToolObject Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CToolObject::Clone(void* pArg)
{
	CToolObject* pInstance = new CToolObject(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
	{
		MSG_BOX(L"StaticObject Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolObject::setPos(_vector vPos)
{
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, vPos);
}

void CToolObject::setRotate(_vector vRotate)
{
	XMStoreFloat3(&m_vRotateValue, vRotate);

	_float fRadianX, fRadianY, fRadianZ;

	fRadianX = XMConvertToRadians(XMVectorGetX(vRotate));
	fRadianY = XMConvertToRadians(XMVectorGetY(vRotate));
	fRadianZ = XMConvertToRadians(XMVectorGetZ(vRotate));

	m_pTransform->Set_Rotate(XMVectorSet(1.f, 0.f, 0.f, 0.f), fRadianX);
	m_pTransform->Set_Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadianY);
	m_pTransform->Set_Rotate(XMVectorSet(0.f, 0.f, 1.f, 0.f), fRadianZ);
}

void CToolObject::setScale(_vector vScale)
{
	m_pTransform->Set_Scale(vScale);
}
void CToolObject::Change_TransformValue(_vector& vScale, _vector& vRotate, _vector& vPos)
{
	vScale = m_pTransform->getScale();
	vRotate = XMLoadFloat3(&m_vRotateValue);
	vPos =m_pTransform->getState(CTransform::TRANS_STATE::STATE_POS);
}
void CToolObject::Change_ColliderValue(_uint iIndex, _vector& vScale, _vector& vRotate, _vector& vPos)
{
	vScale = m_vecCollider[iIndex]->pCollider->getExtends();
	vRotate = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vPos = m_vecCollider[iIndex]->pCollider->getCenter();
}

void CToolObject::setColliderValue(_uint iIndex, _fvector vScale, _fvector vRotate, _fvector vPos)
{
	m_vecCollider[iIndex]->pCollider->setScale(vScale);
	m_vecCollider[iIndex]->pCollider->setCenter(vPos);
}

void CToolObject::ResetColliderColor()
{
	for (auto& pCollider : m_vecCollider)
		pCollider->pCollider->setColor(XMVectorSet(0.f, 0.f, 0.f, 1.f));
}

void CToolObject::setMatrix(_fmatrix matTransform)
{
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_RIGHT, matTransform.r[0]);
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_UP, matTransform.r[1]);
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_LOOK, matTransform.r[2]);
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, matTransform.r[3]);
}

void CToolObject::AddBoneCollider(const char* pBoneName)
{
	CHierarchyNode* pBone = m_pModel->getBone(pBoneName);
	if (!pBone)
		return;
	m_vecBone.emplace_back(pBone);
	Safe_AddRef(pBone);
}

void CToolObject::DeleteCollider(_uint iIndex)
{
	if (!m_vecCollider[iIndex]->bBone)
	{
		Safe_Release(m_vecBone[iIndex - 1]);
		auto iter = m_vecBone.begin();
		m_vecBone.erase(iter + iIndex - 1);
	}
	Safe_Release(m_vecCollider[iIndex]->pCollider);
	Safe_Delete(m_vecCollider[iIndex]);

	auto iter = m_vecCollider.begin();
	m_vecCollider.erase(iter + iIndex);
}

HRESULT CToolObject::Save_Collider(const _tchar* pFileName)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	vector<COLLIDERDATA> saveData;
	
	for(_uint i=0; i<(_uint)m_vecCollider.size(); i++)
	{
		COLLIDERDATA pData;
		ZeroMemory(&pData, sizeof(COLLIDERDATA));

		pData.eType = (_uint)m_vecCollider[i]->pCollider->getType();
		XMStoreFloat3(&pData.vPosition,m_vecCollider[i]->pCollider->getCenter());
		XMStoreFloat3(&pData.vScale,m_vecCollider[i]->pCollider->getExtends());
		pData.bBone = m_vecCollider[i]->bBone;
		if (!pData.bBone)//true¸é World°ö
			strcpy_s(pData.szBone, m_vecBone[i - 1]->Get_Name());

		saveData.emplace_back(pData);
	}

	if (FAILED(pInstance->SaveFile<COLLIDERDATA>(&saveData, pFileName)))
	{
		MSG_BOX(L"Collider Save Fail");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
		
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolObject::Load_Collider(const _tchar* pFileName)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	vector<COLLIDERDATA> pVecData;

	if (FAILED(pInstance->LoadFile<COLLIDERDATA>(pVecData, pFileName)))
	{
		MSG_BOX(L"Collider Load Fail");
		RELEASE_INSTANCE(CGameInstance)
		return E_FAIL;
	}
	
	for (auto& pCollider : m_vecCollider)
	{
		Safe_Release(pCollider->pCollider);
		Safe_Delete(pCollider);
	}
	m_vecCollider.clear();
	m_vecCollider.shrink_to_fit();

	m_vecCollider.reserve((_uint)pVecData.size());

	for (auto& pData : pVecData)
	{
		CCollider::COLLIDERDESC tDesc;
		ZeroMemory(&tDesc, sizeof(CCollider::COLLIDERDESC));

		tDesc.eType = (CCollider::COLLIDER_TYPE)pData.eType;
		tDesc.vScale = _float3(1.f, 1.f, 1.f);
		tDesc.vPosition = _float3(0.f, 0.f, 0.f);

		_uint iIndexNum = 0;
		CCollider* pCollider = Init_ComDeForm<CCollider>(TOOLSCENEID::TOOL_DEFORM, COMPONENTID::COLLIDER, &iIndexNum, &tDesc);
		
		pCollider->setCenter(XMLoadFloat3(&pData.vPosition));
		pCollider->setScale(XMLoadFloat3(&pData.vScale));

		COLLIDER* newCollider = new COLLIDER;

		newCollider->bBone = pData.bBone;
		if (!newCollider->bBone)
		{
			CHierarchyNode* pBone=m_pModel->getBone(pData.szBone);
			m_vecBone.push_back(pBone);
		}
		newCollider->pCollider = pCollider;

		m_vecCollider.emplace_back(newCollider);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_fmatrix CToolObject::Calcul_BonMatrix(CHierarchyNode* pBone)
{
	_matrix matCombine = pBone->Get_CombinedMatrix();
	_matrix matPivot = m_pModel->Get_PivotMatirx();
	_matrix matBone = matCombine * matPivot * m_pTransform->getWorldMatrix();
	
	return matBone;
}


void CToolObject::Free()
{
	CGameObject::Free();
	Safe_Release(m_pModel);

	for (auto& pCollider : m_vecCollider)
	{
		Safe_Release(pCollider->pCollider);
		Safe_Delete(pCollider);
	}
	m_vecCollider.clear();
	m_vecCollider.shrink_to_fit();

	for_each(m_vecBone.begin(), m_vecBone.end(), CDeleteObj());
	m_vecBone.clear();
	m_vecBone.shrink_to_fit();
}

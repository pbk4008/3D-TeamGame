#include "Animator.h"
#include "AnimNode.h"
#include "Animation.h"

CAnimator::CAnimator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pController(nullptr)
	, m_pHead(nullptr)
	, m_pCulAnimNode(nullptr)
	, m_pChangeNode(nullptr)
	, m_bLerp(false)
	, m_eType(CAnimationController::EType::Max)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	: CComponent(rhs)
	, m_pController(rhs.m_pController)
	, m_pHead(nullptr)
	, m_pCulAnimNode(nullptr)
	, m_pChangeNode(nullptr)
	, m_bLerp(false)
	, m_eType(rhs.m_eType)
{
	Safe_AddRef(m_pController);
}

HRESULT CAnimator::NativeConstruct_Prototype()
{
	if (FAILED(CComponent::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimator::NativeConstruct(void* pArg)
{
	if (!pArg)
		return E_FAIL;

	ANIMATORDESC tDesc = (*(ANIMATORDESC*)pArg);

	if (FAILED(CComponent::NativeConstruct(pArg)))
		return E_FAIL;

	m_pController = CAnimationController::Create(m_pDevice, m_pDeviceContext);
	if (!m_pController)
		return E_FAIL;

	m_eType = tDesc.eType;
	m_pController->Set_Model(tDesc.pModel);
	m_pController->Set_Transform(tDesc.pTransform);
	m_pController->Set_MoveSpeed(40.f);

	m_pHead = CAnimNode::Create(0, nullptr, false,false,false,ERootOption::Max);
	m_vecAnimNode.emplace_back(m_pHead->Get_Index());
	if (!m_pHead)
		return E_FAIL;

	m_pCulAnimNode = m_pHead;

	return S_OK;
}

_int CAnimator::Tick(_double dDeltaTime)
{
	m_pController->Tick(dDeltaTime, m_eType);
	if (m_pController->Get_ChangeAnimation())
	{
		m_pCulAnimNode = m_pChangeNode;
		m_pController->Set_IsChange(false);
		m_bLerp = false;
	}

	if (m_pCulAnimNode->Get_AutoIndex() != -1)
	{
		if (!m_pCulAnimNode->Get_Loop() && m_pController->Is_Finished())
			Change_Animation(m_pCulAnimNode->Get_AutoIndex());
		if (m_pCulAnimNode->Is_LoopChange())
			m_pCulAnimNode->Change_Loop(true);
		//if (m_pController->Is_Finished())
		//	Change_Animation(m_vecAnimNodeName[m_pCulAnimNode->Get_AutoIndex()]);
	}
	return _int();
}

_int CAnimator::LateTick(_double dDeltaTime)
{
	return _int();
}

HRESULT CAnimator::Insert_Animation(_uint iTag, _uint iConnectTag, CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, _bool bLoop, ERootOption eOption, _bool bDouble)
{
	//만들고자 하는 애니메이션 중복 체크
	if (Get_DuplicateTag(iTag))
		return E_FAIL;
	
	//애니메이션으로 AnimNode 만들기
	CAnimNode* pNewNode = CAnimNode::Create(iTag, pAnim, bLoop, bRootAnim,bTransFrom,eOption);
	if (!pNewNode)
		return E_FAIL;

	m_vecAnimNode.emplace_back(iTag);
	
	//AnimNode 연결
	if (FAILED(Connect_Animation(iConnectTag, pNewNode, bDouble)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimator::Connect_Animation(_uint iConnectTag, CAnimNode* pNode, _bool bDouble)
{
	CAnimNode* pConntectNode = Find_Animation(iConnectTag);

	if (!pConntectNode)
		return E_FAIL;

	pConntectNode->Conntect_AnimNode(pNode);
	if (bDouble)
		pNode->Conntect_AnimNode(pConntectNode);

	return S_OK;
}
HRESULT CAnimator::Connect_Animation(_uint iTag, _uint iConnectTag, _bool bDouble)
{
	CAnimNode* pNode = Find_Animation(iTag);
	if (!pNode)
		return E_FAIL;

	CAnimNode* pConntectNode = Find_Animation(iConnectTag);

	if (!pConntectNode)
		return E_FAIL;

	pConntectNode->Conntect_AnimNode(pNode);

	if (bDouble)
		pNode->Conntect_AnimNode(pConntectNode);

	return S_OK;
}
HRESULT CAnimator::Set_UpAutoChangeAnimation(_uint iTag, _uint iEndTag)
{
	CAnimNode* pNode = Find_Animation(iTag);
	if (!pNode)
		return E_FAIL;

	CAnimNode* pEndNode = Find_Animation(iEndTag, pNode);
	if (!pEndNode)
		return E_FAIL;

	pNode->Set_AutoChangeIndex(pEndNode->Get_Index());

	return S_OK;
}

HRESULT CAnimator::Insert_AnyEntryAnimation(_uint iTag)
{
	//애니메이터 루프내에 애니메이션이 있는지 없는지 판단
	//있어야만 루프 내의 애니메이션에서 가져올수 있음
	if (!Get_DuplicateTag(iTag))
		return E_FAIL;
	//AnyEntry내에 해당 태그를 가지고 있는 애니메이션 존재하는지 판단
	if (Get_AnyEntryDuplicateTag(iTag))
		return E_FAIL;

	//애니메이션 노드 찾기
	CAnimNode* pNode = Find_Animation(iTag);
	if (!pNode)
		return E_FAIL;

	//찾으면 AnyEntry에 넣어주기
	m_vecAnyEntryNode.emplace_back(pNode);

	return S_OK;
}

void CAnimator::Set_PivotMat(const _fmatrix& matPivot)
{
	m_pController->Set_PivotMatrix(matPivot);
}

void CAnimator::Add_AnimFrame(_uint iTag, _uint iFrame)
{
	CAnimNode* pAnimNode = Find_Animation(iTag);

	if (!pAnimNode)
		MSGBOX("AnimNode Null");

	pAnimNode->Get_Animation()->Add_TrackAcc(iFrame);
}


const _uint CAnimator::Get_CurrentAnimNode()
{
	if (!m_pCulAnimNode)
		return -1;

	return m_pCulAnimNode->Get_Index();
}

CAnimation* CAnimator::Get_CurrentAnimation()
{
	if (!m_pCulAnimNode)
		return nullptr;

	return m_pCulAnimNode->Get_Animation();
}

HRESULT CAnimator::Change_Animation(_uint iTag)
{
	//현재 애니메이션과 연결된 애들 중에서 태그 값을 가진 애니메이션노드 찾기
	CAnimNode* pNode = m_pCulAnimNode->Check_ConnectNode(iTag);

	//없으면 끝
	if (!pNode)
	{
		MSGBOX("현재 연결된 애니메이션이 아닙니다.");
		return S_OK;
	}
	//있으면 바꿀애니메이션으로 변경
	m_pChangeNode = pNode;

	//Animation컨트롤러에 바꿀애니메이션으로 예약
	if (FAILED(m_pController->SetUp_NextAnimation(m_pChangeNode)))
		return E_FAIL;
	m_bLerp = true;
	return S_OK;
}

HRESULT CAnimator::Change_AnyEntryAnimation(_uint iTag)
{
	CAnimNode* pNode = Find_AnyEntryAnim(iTag);
	if (!pNode)
		return E_FAIL;

	m_pChangeNode = pNode;

	if (FAILED(m_pController->SetUp_NextAnimation(m_pChangeNode)))
		return E_FAIL;

	m_bLerp = true;
	return S_OK;
}

HRESULT CAnimator::Change_LoopAnim()
{
	if (!m_pCulAnimNode)
		return E_FAIL;

	if (!m_pCulAnimNode->Get_Loop() && m_pCulAnimNode->Get_AutoIndex() != -1)
		return S_OK;

	m_pCulAnimNode->Change_Loop(false);

	return S_OK;
}

void CAnimator::Set_PivotMatrix(_fmatrix matPivot)
{
	if (!m_pController)
		return;
	m_pController->Set_PivotMatrix(matPivot);
}

CAnimNode* CAnimator::Find_Animation(_uint iTag, CAnimNode* pNode)
{
	if (iTag == 0)
		return m_pHead;

	if (!Get_DuplicateTag(iTag))
		return nullptr;

	if (!pNode)
		pNode = m_pHead;

	if (pNode->Is_LinkEmpty())
		return nullptr;

	vector<_uint> vecFind;
	for (auto& pNode : m_vecAnimNode)
		vecFind.emplace_back(pNode);

	return pNode->Check_ConnectNode(iTag,&vecFind);
}


CAnimNode* CAnimator::Find_AnyEntryAnim(_uint iTag)
{
	CAnimNode* pFindNode = nullptr;

	for (auto& pNode : m_vecAnyEntryNode)
	{
		if (pNode->Get_Index() == iTag)
		{
			pFindNode = pNode;
			break;
		}
	}
	return pFindNode;
}

_bool CAnimator::Get_DuplicateTag(_uint iAnimTag)
{
	for (auto& pIndex : m_vecAnimNode)
	{
		if (pIndex == iAnimTag)
			return true;
	}
	return false;
}

_bool CAnimator::Get_AnyEntryDuplicateTag(_uint iAnimTag)
{
	for (auto& pNode : m_vecAnyEntryNode)
	{
		if (pNode->Get_Index() == iAnimTag)
			return true;
	}
	return false;
}

CAnimator* CAnimator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CAnimator* pInstance = new CAnimator(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CAnimator Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void* pArg)
{
	CAnimator* pInstance = new CAnimator(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CAnimator Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	CComponent::Free();
	Safe_Release(m_pController);
	m_pHead->Delete_Node(m_vecAnimNode);
	Safe_Release(m_pHead);

	m_vecAnyEntryNode.clear();
}

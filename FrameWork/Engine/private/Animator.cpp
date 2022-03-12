#include "Animator.h"
#include "AnimationController.h"
#include "AnimNode.h"
#include "Animation.h"

CAnimator::CAnimator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pController(nullptr)
	, m_pHead(nullptr)
	, m_pCulAnimNode(nullptr)
	,m_pChangeNode(nullptr)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	: CComponent(rhs)
	, m_pController(rhs.m_pController)
	, m_pHead(nullptr)
	, m_pCulAnimNode(nullptr)
	, m_pChangeNode(nullptr)
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

	m_pController->Set_Model(tDesc.pModel);
	m_pController->Set_Transform(tDesc.pTransform);

	m_pHead = CAnimNode::Create(L"Head",nullptr, false,(_uint)m_vecAnimNodeName.size(),false,false,ERootOption::Max);
	m_vecAnimNodeName.emplace_back(m_pHead->Get_Name());

	if (!m_pHead)
		return E_FAIL;

	m_pCulAnimNode = m_pHead;


	return S_OK;
}

_int CAnimator::Tick(_double dDeltaTime)
{
	m_pController->Tick(dDeltaTime);
	if (m_pCulAnimNode->Get_AutoIndex() != -1)
	{
		if (m_pController->Is_Finished())
			Change_Animation(m_vecAnimNodeName[m_pCulAnimNode->Get_AutoIndex()]);
	}
	return _int();
}

_int CAnimator::LateTick(_double dDeltaTime)
{
	return _int();
}

HRESULT CAnimator::Insert_Animation(const wstring& pName, const wstring& pConnectName, class CAnimation* pAnim, _bool bRootAnim, _bool bTransFrom, ERootOption eOption, _bool bDouble)
{
	//만들고자 하는 애니메이션 중복 체크
	auto& iter = find(m_vecAnimNodeName.begin(), m_vecAnimNodeName.end(), [&](wstring tName)
		{
			if (tName == pConnectName)
				return true;
			return false;
		});
	
	if (iter != m_vecAnimNodeName.end())
		return E_FAIL;

	//애니메이션으로 AnimNode 만들기
	CAnimNode* pNewNode = CAnimNode::Create(pName, pAnim, pAnim->Get_Loop(), (_uint)m_vecAnimNodeName.size(),bRootAnim,bTransFrom,eOption);
	if (!pNewNode)
		return E_FAIL;
	m_vecAnimNodeName.emplace_back(pName);
	
	//AnimNode 연결
	if (FAILED(Connect_Animation(pConnectName, pNewNode, bDouble)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimator::Connect_Animation(const wstring& pConnectName, CAnimNode* pNode, _bool bDouble)
{
	CAnimNode* pConntectNode = Find_Animation(pConnectName);

	if (!pConntectNode)
		return E_FAIL;

	pConntectNode->Conntect_AnimNode(pNode);
	if (bDouble)
		pNode->Conntect_AnimNode(pConntectNode);

	return S_OK;
}
HRESULT CAnimator::Set_UpAutoChangeAnimation(const wstring& pAnim, const wstring& pEndAnim)
{
	CAnimNode* pNode = Find_Animation(pAnim);
	if (!pNode)
		return E_FAIL;
	CAnimNode* pEndNode = Find_Animation(pEndAnim, pNode);
	if (!pEndNode)
		return E_FAIL;

	pNode->Set_AutoChangeIndex(pEndNode->Get_Index());

	return S_OK;
}

const wstring& CAnimator::Get_CurrentAnim()
{
	if (!m_pCulAnimNode)
		return nullptr;

	return m_pCulAnimNode->Get_Name();
}

HRESULT CAnimator::Change_Animation(const wstring& pName)
{
	m_pChangeNode = m_pCulAnimNode->Check_ConnectNode(pName);
	if (!m_pChangeNode)
		return E_FAIL;

	if (FAILED(m_pController->SetUp_NextAnimation(m_pChangeNode)))
		return E_FAIL;

	return S_OK;
}

CAnimNode* CAnimator::Find_Animation(const wstring& pConnectName, CAnimNode* pNode = nullptr)
{
	auto& pFind=find(m_vecAnimNodeName.begin(), m_vecAnimNodeName.end(), [&](wstring tName)
					{
						if (tName == pConnectName)
							return true;
						return false;
					});

	if (pFind == m_vecAnimNodeName.end())
		return nullptr;

	if (!pNode)
		pNode = m_pHead;

	if (pNode->Is_LinkEmpty())
		return nullptr;

	return pNode->Check_ConnectNode(pConnectName);
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
	Safe_Release(m_pHead);
}

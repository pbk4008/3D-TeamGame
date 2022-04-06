#include "StateController.h"

#include "GameInstance.h"
#include "State.h"

CStateController::CStateController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
{
}

CStateController::CStateController(const CStateController& _rhs)
	: CComponent(_rhs)
{
}

HRESULT CStateController::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStateController::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CStateController::Tick(const _double& _dDeltaTime)
{
	if (!m_pCurState)
		return -1;
	if(m_isChange)
		m_isChange = false;

	_int iProgress = m_pCurState->Tick(_dDeltaTime);
	if (0 != iProgress)
		return iProgress;

	return _int();
}

_int CStateController::LateTick(const _double& _dDeltaTime)
{
	if (!m_pCurState)
		return -1;
	if (m_isChange)
		return NO_EVENT;

	_int iProgress = m_pCurState->LateTick(_dDeltaTime);
	if (0 != iProgress)
		return iProgress;

	return _int();
}

HRESULT CStateController::Render()
{
	if (m_isChange)
		return NO_EVENT;

	if (FAILED(m_pCurState->Render()))
		return E_FAIL;

	return S_OK;
}

CGameObject* CStateController::Get_GameObject() const
{
	return m_pGameObject;
}

unordered_map<wstring, CState*>& CStateController::Get_States()
{
	return m_mapStates;
}

CState* CStateController::Get_CurState()
{
	return m_pCurState;
}

CState* CStateController::Get_PreState()
{
	return m_pPreState;
}

const wstring& CStateController::Get_CurStateTag() const
{
	return m_wstrCurStateTag;
}

void CStateController::Set_GameObject(CGameObject* _pGameObject)
{
	m_pGameObject = _pGameObject;
}

HRESULT CStateController::Add_State(const wstring& _wstrStateTag, CState* _pState)
{
	//State ã��
	auto& iter_find = m_mapStates.find(_wstrStateTag);
	if (m_mapStates.end() != iter_find)//������ ������
		return E_FAIL;

	m_mapStates.emplace(_wstrStateTag, _pState);//������ �߰�

	return S_OK;
}

HRESULT CStateController::Change_State(const wstring& _wstrStateTag, const EChange _eChange)
{
	//���� �±װ� �ٲٰ��� �ϴ� �±׶� �ٸ���
	if (m_wstrCurStateTag != _wstrStateTag)
	{
		//�ٲٰ��� �ϴ� ���� ��������
		CState* pState = m_mapStates[_wstrStateTag];
		//�ٲٰ��� �ϴ� ���� ������ ��
		if (!pState)
			return E_FAIL;

		//���� ���°� ������
		if (m_pCurState)
		{
			switch (_eChange)
			{
			case EChange::Normal:
			case EChange::NonEnter://���� ������ ������ �̺�Ʈ�� ȣ��
				m_pCurState->ExitState();
				break;
			} 
		}

		m_pPreState = m_pCurState;//���� ���¸� ���� ���·� �ٲٰ�
		m_pCurState = pState;//���� ���¸� �ٲٰ��� �ϴ� ���·� ����

		if (m_pCurState)
		{
			switch (_eChange)
			{
			case EChange::Normal:
			case EChange::NonExit://��������� ������ �̺�Ʈ�� ȣ��
				m_pCurState->EnterState();
				break;
			}
		}

		m_wstrPreStateTag = m_wstrCurStateTag;//���� �±׸� ���� �±׷� ����
		m_wstrCurStateTag = _wstrStateTag;//���� �±׸� �ٲ� �±׷� ����

		m_isChange = true;//���¸� �مf�ٶ�� ǥ���ض�(�������� �ٽ� ���� -> LateTick ���� ���� ���¸� ���ƾ� �ϱ� ����)
	}

	return S_OK;
}

HRESULT CStateController::Change_State(const wstring& _wstrStateTag, void* _pArg, const EChange _eChange)
{
	//�Ű� ���� ������ ���¸� ���� �ϴ� �Լ�
	if (m_wstrCurStateTag != _wstrStateTag)
	{
		//�ٲٰ��� �ϴ� ���� ��������
		CState* pState = m_mapStates[_wstrStateTag];
		//�ٲٰ��� �ϴ� ���� ������ ��
		if (!pState)
		{
			return E_FAIL;
		}

		//���� ���°� ������
		if (m_pCurState)
		{
			switch (_eChange)
			{
			case EChange::Normal:
			case EChange::NonEnter://���� ������ ������ �̺�Ʈ�� ȣ��
				m_pCurState->ExitState(_pArg);
				break;
			}
		}

		m_pPreState = m_pCurState;//���� ���¸� ���� ���·� �ٲٰ�
		m_pCurState = pState;//���� ���¸� �ٲٰ��� �ϴ� ���·� ����

		if (m_pCurState)
		{
			switch (_eChange)
			{
			case EChange::Normal:
			case EChange::NonExit://��������� ������ �̺�Ʈ�� ȣ��
				m_pCurState->EnterState(_pArg);
				break;
			}
		}

		m_wstrPreStateTag = m_wstrCurStateTag;//���� �±׸� ���� �±׷� ����
		m_wstrCurStateTag = _wstrStateTag;//���� �±׸� �ٲ� �±׷� ����

		m_isChange = true;//���¸� �مf�ٶ�� ǥ���ض�(�������� �ٽ� ���� -> LateTick ���� ���� ���¸� ���ƾ� �ϱ� ����)
	}

	return S_OK;
}

void CStateController::OnCollisionEnter(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnCollisionEnter(collision);
}

void CStateController::OnCollisionStay(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnCollisionStay(collision);
}

void CStateController::OnCollisionExit(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnCollisionExit(collision);
}

void CStateController::OnTriggerEnter(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnTriggerEnter(collision);
}

void CStateController::OnTriggerStay(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnTriggerStay(collision);
}

void CStateController::OnTriggerExit(CCollision& collision)
{
	if (m_pCurState)
		m_pCurState->OnTriggerExit(collision);
}

CStateController* CStateController::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStateController* pInstance = new CStateController(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CStateController Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CStateController::Clone(void* _pArg)
{
	CStateController* pInstance = new CStateController(*this);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CStateController Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStateController::Free()
{
	for (auto& pState : m_mapStates)
		Safe_Release(pState.second);
	m_mapStates.clear();

	__super::Free();
}

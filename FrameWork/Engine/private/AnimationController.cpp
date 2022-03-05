#include "AnimationController.h"

#include "GameInstance.h"
#include "Animation.h"

CAnimationController::CAnimationController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
{
}

CAnimationController::CAnimationController(const CAnimationController& _rhs)
	: CComponent(_rhs)
{
}

HRESULT CAnimationController::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAnimationController::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CAnimationController::Tick(const _double& _dDeltaTime)
{
	if (0 > Update_CombinedTransformMatrix(_dDeltaTime))
	{
		return -1;
	}

	if (0 > Move_Transform(_dDeltaTime))
	{
		return -1;
	}

	return _int();
}

_int CAnimationController::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CAnimationController::Render()
{
	Render_Debug();

	return S_OK;
}

const _int CAnimationController::Get_CurAnimIndex() const
{
	return m_tBlendDesc.iCurAnimIndex;
}

const ERootOption CAnimationController::Get_RootOption() const
{
	return m_eRootOption;
}

void CAnimationController::Set_GameObject(CGameObject* _pGameObject)
{
	m_pGameObject = _pGameObject;
}

void CAnimationController::Set_Transform(CTransform* _pTransform)
{
	m_pTransform = _pTransform;
}

void CAnimationController::Set_Model(CModel* _pModel)
{
	m_pModel = _pModel;
}

void CAnimationController::Set_LoopAnim(const _bool _isLoop)
{
	m_isLoopAnim = _isLoop;
}

void CAnimationController::Set_TransformMove(const _bool _isTransformMove)
{
	m_isTransformMove = _isTransformMove;
}

void CAnimationController::Set_PivotMatrix(const _fmatrix& _smatPivot)
{
	m_smatPivot = _smatPivot;
}

void CAnimationController::Set_RootMotion(const _bool _isRootMotion, const _bool _isTransformMove, const ERootOption _eRootOption)
{
	m_isRootMotion = _isRootMotion;
	m_isTransformMove = _isTransformMove;
	m_eRootOption = _eRootOption;
}

const _bool CAnimationController::Is_RootMotion() const
{
	return m_isRootMotion;
}

const _bool CAnimationController::Is_Finished() const
{
	return m_isFinished;
}

_int CAnimationController::Update_CombinedTransformMatrix(const _double& _dDeltaTime)
{
	vector<CAnimation*>& vecAnimations = m_pModel->Get_Animations();
	vecAnimations[m_tBlendDesc.iCurAnimIndex]->Update_TransformationMatrix(_dDeltaTime, m_isLoopAnim);

	if (m_tBlendDesc.fTweenTime >= 1.f)
	{
		vecAnimations[m_tBlendDesc.iCurAnimIndex]->Reset_Animation();

		m_tBlendDesc.iCurAnimIndex = m_tBlendDesc.iNextAnimIndex;
		m_isLoopAnim = m_tBlendDesc.isLoopNextAnim;
		m_tBlendDesc.iNextAnimIndex = -1;
		m_tBlendDesc.fChangeTime = 0.f;
		m_tBlendDesc.fTweenTime = 0.f;
		m_isChangeAnim = false;

		m_pPreAnim = m_pCurAnim;
		m_pCurAnim = vecAnimations[m_tBlendDesc.iCurAnimIndex];

		m_strPreAnimTag = m_strCurAnimTag;
		m_strCurAnimTag = vecAnimations[m_tBlendDesc.iCurAnimIndex]->Get_Name();
	}

	if (-1 != m_tBlendDesc.iNextAnimIndex)
	{
		if (!m_isChangeAnim)
		{
			vecAnimations[m_tBlendDesc.iNextAnimIndex]->Reset_Animation();
			m_isChangeAnim = true;
		}

		m_tBlendDesc.fChangeTime += (_float)_dDeltaTime;
		m_tBlendDesc.fTweenTime = m_tBlendDesc.fChangeTime / m_tBlendDesc.fTakeTime;

		vecAnimations[m_tBlendDesc.iNextAnimIndex]->Update_TransformationMatrix(_dDeltaTime, m_isLoopAnim);
		m_isFinished = vecAnimations[m_tBlendDesc.iNextAnimIndex]->Is_Finished();

		Lerp_Anim(vecAnimations);
	}
	else
	{
		m_isFinished = vecAnimations[m_tBlendDesc.iCurAnimIndex]->Is_Finished();
	}

	return _int();
}

void CAnimationController::Lerp_Anim(vector<CAnimation*>& _vecvecAnimations)
{
	vector<CChannel*>& vecCurrentAnim = _vecvecAnimations[m_tBlendDesc.iCurAnimIndex]->Get_Channels();
	vector<CChannel*>& vecNextAnim = _vecvecAnimations[m_tBlendDesc.iNextAnimIndex]->Get_Channels();

	vector<_matrix> vecInterPolMatrix;
	vecInterPolMatrix.reserve(vecCurrentAnim.size());

	_vector vScale{};
	_vector vRotation{};
	_vector vPosition{};

	for (size_t i = 0; i < vecNextAnim.size(); ++i)
	{
		_vector		vSourScale, vSourRotation, vSourPosition;
		_vector		vDestScale, vDestRotation, vDestPosition;

		{
			/* 현재 키프레임의 상태 값 */
			vSourScale = XMLoadFloat4(&vecCurrentAnim[i]->getAnimInterPolation().vScale);
			vSourRotation = XMLoadFloat4(&vecCurrentAnim[i]->getAnimInterPolation().vRotation);
			vSourPosition = XMLoadFloat4(&vecCurrentAnim[i]->getAnimInterPolation().vPosition);

			/* 다음 키프레임의 상태 값 */
			vDestScale = XMLoadFloat4(&vecNextAnim[i]->getAnimInterPolation().vScale);
			vDestRotation = XMLoadFloat4(&vecNextAnim[i]->getAnimInterPolation().vRotation);
			vDestPosition = XMLoadFloat4(&vecNextAnim[i]->getAnimInterPolation().vPosition);

			/* 계산된 비율 만큼 현재 키프레임과 다음 키프레임의 상태값을 보간 */
			vScale = XMVectorLerp(vSourScale, vDestScale, m_tBlendDesc.fTweenTime);
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, m_tBlendDesc.fTweenTime);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, m_tBlendDesc.fTweenTime);

			_matrix smatTransform = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
			vecCurrentAnim[i]->Set_TransformationMatrix(smatTransform);
		}
	}
}

HRESULT CAnimationController::SetUp_NextAnimation(const string& _strAnimTag, const _bool _isLoopNextAnim)
{
	if (m_strCurAnimTag != _strAnimTag)
	{
		vector<CAnimation*>& vecAnimations = m_pModel->Get_Animations();

		for (auto& pAnimation : vecAnimations)
		{
			if (!strcmp(_strAnimTag.c_str(), pAnimation->Get_Name()))
			{
				m_tBlendDesc.iNextAnimIndex = pAnimation->Get_Index();
				m_tBlendDesc.isLoopNextAnim = _isLoopNextAnim;
				m_pFixedBone = pAnimation->Get_Channel("root");
				return S_OK;
			}
		}
	}

	return S_OK;
}

const _int CAnimationController::Move_Transform(const _double& _dDeltaTime)
{
	if (!m_pTransform)
	{
		return -1;
	}
	if (!m_isTransformMove)
	{
		return 0;
	}

	if (m_pCurAnim)
	{
		if (m_pFixedBone)
		{
			vector<KEYFRAME*> KeyFrames = m_pFixedBone->Get_KeyFrames();
			m_iCurKeyFrameIndex = m_pFixedBone->Get_CurrentKeyFrameIndex();
			m_iPreKeyFrameIndex = m_iCurKeyFrameIndex - 1;

			_vector svPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
			_vector svRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
			_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
			_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);


			_vector svPreVelocity = XMVectorZero();
			_vector svVelocity = XMVectorZero();

			_vector svPreQuaternian = XMVectorZero();
			_vector svQuaternian = XMVectorZero();

			if (0 == m_iCurKeyFrameIndex)
			{
				svPreVelocity = XMLoadFloat3(&KeyFrames[KeyFrames.size() - 2]->vPosition);
				svVelocity = XMLoadFloat3(&KeyFrames[KeyFrames.size() - 1]->vPosition);

				svPreQuaternian = XMLoadFloat4(&KeyFrames[KeyFrames.size() - 2]->vRotation);
				svQuaternian = XMLoadFloat4(&KeyFrames[KeyFrames.size() - 1]->vRotation);

				svVelocity -= svPreVelocity;
				svQuaternian -= svPreQuaternian;
			}
			else
			{
				if (m_iPreKeyFrameIndex >= 0)
				{
					svPreVelocity = XMLoadFloat3(&KeyFrames[m_iPreKeyFrameIndex]->vPosition);
					svPreQuaternian = XMLoadFloat4(&KeyFrames[m_iPreKeyFrameIndex]->vRotation);
				}
				svVelocity = XMLoadFloat3(&KeyFrames[m_iCurKeyFrameIndex]->vPosition);
				svQuaternian = XMLoadFloat4(&KeyFrames[m_iCurKeyFrameIndex]->vRotation);

				svVelocity -= svPreVelocity;
				svQuaternian -= svPreQuaternian;
			}

			svVelocity *= 0.2f;
			svQuaternian = XMVector4Transform(svQuaternian, m_smatPivot);

			_float3 vVelocity, vBonePosition, vEuler, vRotation;
			_float4 vQuaternian;
			XMStoreFloat3(&vVelocity, svVelocity);
			XMStoreFloat4(&vQuaternian, svQuaternian);
			vEuler = QuaternionToEuler(vQuaternian);

			switch (m_eRootOption)
			{
			case ERootOption::X:
				vBonePosition = { -vVelocity.x, 0.f, 0.f };
				break;
			case ERootOption::Y:
				vBonePosition = { 0.f, 0.f, -vVelocity.y };
				break;
			case ERootOption::Z:
				vBonePosition = { 0.f, -vVelocity.z, 0.f };
				break;
			case ERootOption::XY:
				vBonePosition = { -vVelocity.x, 0.f, -vVelocity.y };
				break;
			case ERootOption::XZ:
				vBonePosition = { -vVelocity.x, -vVelocity.z, 0.f };
				break;
			case ERootOption::YZ:
				vBonePosition = { 0.f, -vVelocity.z, -vVelocity.y };
				break;
			case ERootOption::XYZ:
				vBonePosition = { -vVelocity.x, -vVelocity.z, -vVelocity.y };
				break;
			}
			vRotation = { -vEuler.x, -vEuler.z, -vEuler.y };

			svVelocity = XMLoadFloat3(&vBonePosition);
			svVelocity = XMVector4Transform(svVelocity, m_smatPivot);
			svPosition += svVelocity;

			m_pTransform->Set_State(CTransform::STATE_POSITION, svPosition);
			m_pTransform->Rotation_Axis(svRight, _dDeltaTime * vRotation.x);
			m_pTransform->Rotation_Axis(svUp, _dDeltaTime * vRotation.y);
			m_pTransform->Rotation_Axis(svLook, _dDeltaTime * vRotation.z);



			// 요 아래는 디버그 용이야
			_float3 vPosition = { 0.f, 0.f, 0.f };
			XMStoreFloat3(&vBonePosition, svPosition);

			wstring wstrBonePosition = L"FixedBonePosition : ";
			wstrBonePosition += wstrBonePosition + to_wstring(vBonePosition.x) + L", " + to_wstring(vBonePosition.y) + L", " + to_wstring(vBonePosition.z);
			wstring wstrPosition = L"Position : ";
			wstrPosition += to_wstring(vPosition.x) + L", " + to_wstring(vPosition.y) + L", " + to_wstring(vPosition.z);
		}
	}

	return _int();
}

void CAnimationController::Render_Debug()
{
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_wstrPosition.c_str(), _float2(0.f, 80.f), _float2(0.8f, 0.8f))))
	{
		return;
	}

	if (m_pFixedBone)
	{
		if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.f, 0.f, 1.f), m_wstrFixedBonePosition.c_str(), _float2(0.f, 40.f), _float2(0.8f, 0.8f))))
		{
			return;
		}

		m_wstrPreIndex = (to_wstring(m_iPreKeyFrameIndex));
		if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_wstrPreIndex.c_str(), _float2(0.f, 120.f), _float2(0.8f, 0.8f))))
		{
			return;
		}
		m_wstrCurIndex = (to_wstring(m_iCurKeyFrameIndex));
		if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_wstrCurIndex.c_str(), _float2(0.f, 160.f), _float2(0.8f, 0.8f))))
		{
			return;
		}
	}

	if (m_pCurAnim)
	{
		m_wstrCurAnimTag.assign(m_strCurAnimTag.begin(), m_strCurAnimTag.end());
		m_wstrCurAnimTag = m_wstrCurAnimTag.substr(m_wstrCurAnimTag.find_last_of(L"|") + 1);
		if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_wstrCurAnimTag.c_str(), _float2(0.f, 200.f), _float2(0.8f, 0.8f))))
		{
			return;
		}

		if (m_pCurAnim->Is_Finished())
		{
			m_wstrIsFinished = L"true";
		}
		else
		{
			m_wstrIsFinished = L"false";
		}
		if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_wstrIsFinished.c_str(), _float2(0.f, 240.f), _float2(0.8f, 0.8f))))
		{
			return;
		}
	}
}

CAnimationController* CAnimationController::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CAnimationController* pInstance = new CAnimationController(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera_Silvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CAnimationController::Clone(void* _pArg)
{
	CAnimationController* pInstance = new CAnimationController(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CCamera_Silvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimationController::Free()
{

	__super::Free();
}

#include "AnimationController.h"

#include "GameInstance.h"
#include "Animation.h"
#include "AnimNode.h"

CAnimationController::CAnimationController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
{
}

CAnimationController::CAnimationController(const CAnimationController& _rhs)
	: CComponent(_rhs)
	, m_matPivot(_rhs.m_matPivot)
{
}

HRESULT CAnimationController::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());
	return S_OK;
}

HRESULT CAnimationController::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

_int CAnimationController::Tick(const _double& _dDeltaTime, const EType _eType)
{
	if (0 > Update_CombinedTransformMatrix(_dDeltaTime))
		return -1;

	switch (_eType)
	{
	case EType::Transform:
		//트랜스폼 하는곳
		if (0 > Move_Transform(_dDeltaTime))
			return -1;
		break;
	case EType::CharacterController:
		if (0 > Add_TransformVelocity(_dDeltaTime))
			return -1;
		break;
	}

	if(-1 != m_tBlendDesc.iNextAnimIndex)
	{
		//루트 애니메이션
		if (FAILED(m_pModel->Update_CombinedTransformationMatrix(m_tBlendDesc.iNextAnimIndex, m_isRootMotion, m_eRootOption)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pModel->Update_CombinedTransformationMatrix(m_tBlendDesc.iCurAnimIndex, m_isRootMotion, m_eRootOption)))
			return E_FAIL;
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

const _uint CAnimationController::Get_CurAnimIndex() const
{
	return m_tBlendDesc.iCurAnimIndex;
}

const _uint CAnimationController::Get_CurKeyFrameIndex() const
{
	return m_iCurKeyFrameIndex;
}

const _uint CAnimationController::Get_CurFixedBoneKeyFrameIndex() const
{
	return m_iCurFixedBoneKeyFrameIndex;
}

const _uint CAnimationController::Get_MaxKeyFrameIndex() const
{
	return m_iMaxKeyFrameIndex;
}

const ERootOption CAnimationController::Get_RootOption() const
{
	return m_eRootOption;
}

const _bool CAnimationController::Get_ChangeAnimation() const
{
	return m_isChangeAnim;
}

const string& CAnimationController::Get_CurAnimTag() const
{
	return m_strCurAnimTag;
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
	XMStoreFloat4x4(&m_matPivot, _smatPivot);
}

void CAnimationController::Set_RootMotion(const _bool _isRootMotion, const _bool _isTransformMove, const ERootOption _eRootOption)
{
	m_isRootMotion = _isRootMotion;
	m_isTransformMove = _isTransformMove;
	m_eRootOption = _eRootOption;
}

void CAnimationController::Set_PlaySpeed(const _float _fPlaySpeed)
{
	m_fPlaySpeed = _fPlaySpeed;
}

void CAnimationController::Set_MoveSpeed(const _float _fMoveSpeed)
{
	m_fMoveSpeed = _fMoveSpeed;
}

void CAnimationController::Set_RotSpeed(const _float _fRotSpeed)
{
	m_fRotSpeed = _fRotSpeed;
}

void CAnimationController::Set_IsChange(const _bool _bChange)
{
	m_isChangeAnim = _bChange;
}

const _bool CAnimationController::Is_RootMotion() const
{
	return m_isRootMotion;
}

const _bool CAnimationController::Is_Finished() const
{
	return m_isFinished;
}

void CAnimationController::Add_TrackAcc(const _double& _dTrackAcc)
{
	if (-1 != m_tBlendDesc.iNextAnimIndex)
	{
		vector<CAnimation*>& vecAnimations = m_pModel->Get_Animations();
		vecAnimations[m_tBlendDesc.iNextAnimIndex]->Add_TrackAcc(_dTrackAcc);
		return;
	}
	m_pCurAnim->Add_TrackAcc(_dTrackAcc);
}

_int CAnimationController::Update_CombinedTransformMatrix(const _double& _dDeltaTime)
{
	//애니메이션들 가져옴
	vector<CAnimation*>& vecAnimations = m_pModel->Get_Animations();

	//바꾸는 애니메이션이 잇는지 
	if (-1 != m_tBlendDesc.iNextAnimIndex)
	{
		//if (!m_isChangeAnim)
		//{
		//	vecAnimations[m_tBlendDesc.iNextAnimIndex]->Reset_Animation();
		//	m_isChangeAnim = true;
		//}

		//러프 비율
		m_tBlendDesc.fChangeTime += (_float)_dDeltaTime;
		m_tBlendDesc.fTweenTime = m_tBlendDesc.fChangeTime / m_tBlendDesc.fTakeTime;

		vecAnimations[m_tBlendDesc.iNextAnimIndex]->Update_TransformationMatrix(_dDeltaTime * m_fPlaySpeed, m_tBlendDesc.isLoopNextAnim);
		m_isFinished = vecAnimations[m_tBlendDesc.iNextAnimIndex]->Is_Finished();
		m_iCurKeyFrameIndex = vecAnimations[m_tBlendDesc.iNextAnimIndex]->Get_CurrentKeyFrameIndex();

		//러프 ㄱㄱ
		Lerp_Anim(vecAnimations);
	}
	else
	{
		vecAnimations[m_tBlendDesc.iCurAnimIndex]->Update_TransformationMatrix(_dDeltaTime * m_fPlaySpeed, m_isLoopAnim);
		m_isFinished = vecAnimations[m_tBlendDesc.iCurAnimIndex]->Is_Finished();
		m_iCurKeyFrameIndex = vecAnimations[m_tBlendDesc.iCurAnimIndex]->Get_CurrentKeyFrameIndex();
	}

	//러프의 시간이 1초보다 커지면 애니메이션 변경
	if (m_tBlendDesc.fTweenTime >= 1.f)
	{
		//vecAnimations[m_tBlendDesc.iCurAnimIndex]->Reset_Animation();

		m_tBlendDesc.iCurAnimIndex = m_tBlendDesc.iNextAnimIndex;
		m_isLoopAnim = m_tBlendDesc.isLoopNextAnim;
		m_tBlendDesc.iNextAnimIndex = -1;
		m_tBlendDesc.fChangeTime = 0.f;
		m_tBlendDesc.fTweenTime = 0.f;
		m_isChangeAnim = false;

		m_pPreAnim = m_pCurAnim;
		m_pCurAnim = vecAnimations[m_tBlendDesc.iCurAnimIndex];
		m_iMaxKeyFrameIndex = m_pCurAnim->Get_MaxKeyFrameIndex();

		m_strPreAnimTag = m_strCurAnimTag;
		m_strCurAnimTag = vecAnimations[m_tBlendDesc.iCurAnimIndex]->Get_Name();
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
			vecNextAnim[i]->Set_TransformationMatrix(smatTransform);
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

				vecAnimations[m_tBlendDesc.iNextAnimIndex]->Reset_Animation();

				m_tBlendDesc.isLoopNextAnim = _isLoopNextAnim;
				m_pFixedBone = pAnimation->Get_Channel("root");
				
				m_strPreAnimTag = m_strCurAnimTag;
				m_strCurAnimTag = _strAnimTag;
				m_iCurKeyFrameIndex = pAnimation->Get_CurrentKeyFrameIndex();
				return S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CAnimationController::SetUp_NextAnimation(_uint iIndex, const _bool _isLoopNextAnim)
{
	vector<CAnimation*>& vecAnimations = m_pModel->Get_Animations();
	
	if (vecAnimations.size() < iIndex)
	{
		return E_FAIL;
	}

	if (m_tBlendDesc.iCurAnimIndex != iIndex)
	{
		for (auto& pAnimation : vecAnimations)
		{
			if (iIndex == pAnimation->Get_Index())
			{
				m_tBlendDesc.iNextAnimIndex = pAnimation->Get_Index();

				vecAnimations[m_tBlendDesc.iNextAnimIndex]->Reset_Animation();

				m_tBlendDesc.isLoopNextAnim = _isLoopNextAnim;
				m_pFixedBone = pAnimation->Get_Channel("root");

				m_strPreAnimTag = m_strCurAnimTag;
				m_strCurAnimTag = pAnimation->Get_Name();
				m_iCurKeyFrameIndex = pAnimation->Get_CurrentKeyFrameIndex();
				return S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CAnimationController::SetUp_NextAnimation(class CAnimNode* pChangeAnimNode)
{
	CAnimation* pChangeAnimation = pChangeAnimNode->Get_Animation();

	pChangeAnimation->Reset_Animation();

	m_tBlendDesc.iNextAnimIndex = pChangeAnimation->Get_Index();
	m_tBlendDesc.isLoopNextAnim = pChangeAnimNode->Get_Loop();
	m_pFixedBone = pChangeAnimation->Get_Channel("root");
	
	m_strPreAnimTag = m_strCurAnimTag;
	m_strCurAnimTag = pChangeAnimation->Get_Name();
	m_iCurKeyFrameIndex = pChangeAnimation->Get_CurrentKeyFrameIndex();

	pChangeAnimNode->Set_RootAnimValue(m_isRootMotion, m_isTransformMove, m_eRootOption);
	
	m_isChangeAnim = true;

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
	//현재 애니메이션이 잇으면
	if (m_pCurAnim)
	{
		//루트 본
		if (m_pFixedBone)
		{
			if (m_pFixedBone->Get_CurrentKeyFrameIndex() == m_iCurFixedBoneKeyFrameIndex)
				m_fFixedBoneHoldTime += (_float)_dDeltaTime * m_fPlaySpeed * m_pCurAnim->Get_PalySpeed();
			else
				m_fFixedBoneHoldTime = 0.f;

			if (1.f < m_fFixedBoneHoldTime)
				return 0;

			vector<KEYFRAME*> KeyFrames = m_pFixedBone->Get_KeyFrames();
			m_iCurFixedBoneKeyFrameIndex = m_pFixedBone->Get_CurrentKeyFrameIndex();
			m_iPreFixedBoneKeyFrameIndex = m_iCurFixedBoneKeyFrameIndex - 1;

			_vector svPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
			_vector svRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
			_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
			_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

			_vector svPreVelocity = XMVectorZero();
			_vector svVelocity = XMVectorZero();

			_vector svPreQuaternian = XMVectorZero();
			_vector svQuaternian = XMVectorZero();

			if (0 == m_iCurFixedBoneKeyFrameIndex)
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
				if (m_iPreFixedBoneKeyFrameIndex >= 0)
				{
					svPreVelocity = XMLoadFloat3(&KeyFrames[m_iPreFixedBoneKeyFrameIndex]->vPosition);
					svPreQuaternian = XMLoadFloat4(&KeyFrames[m_iPreFixedBoneKeyFrameIndex]->vRotation);
				}
				svVelocity = XMLoadFloat3(&KeyFrames[m_iCurFixedBoneKeyFrameIndex]->vPosition);
				svQuaternian = XMLoadFloat4(&KeyFrames[m_iCurFixedBoneKeyFrameIndex]->vRotation);

				svVelocity -= svPreVelocity;
				svQuaternian -= svPreQuaternian;
			}

			//svVelocity *= _dDeltaTime;
			//svQuaternian = XMVector4Transform(svQuaternian, XMLoadFloat4x4(&m_matPivot));

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
			vRotation = { vEuler.x, vEuler.z, vEuler.y };

			m_pTransform->Rotation_Axis(svRight, XMConvertToRadians(vRotation.x) * _dDeltaTime * m_fRotSpeed);
			m_pTransform->Rotation_Axis(svUp, XMConvertToRadians(vRotation.y) * _dDeltaTime * m_fRotSpeed);
			m_pTransform->Rotation_Axis(svLook, XMConvertToRadians(vRotation.z) * _dDeltaTime * m_fRotSpeed);

			svVelocity = XMLoadFloat3(&vBonePosition);
			svVelocity = XMVector4Transform(svVelocity, XMLoadFloat4x4(&m_matPivot) * m_pTransform->Get_PivotMatrix());

			XMStoreFloat3(&vVelocity, svVelocity);
			m_pTransform->Go_Right((_float)vVelocity.x * _dDeltaTime * m_fMoveSpeed);
			m_pTransform->Go_Up((_float)vVelocity.y * _dDeltaTime * m_fMoveSpeed);
			m_pTransform->Go_Straight((_float)vVelocity.z * _dDeltaTime * m_fMoveSpeed);
		}
	}

	return _int();
}

const _int CAnimationController::Add_TransformVelocity(const _double& _dDeltaTime)
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
			if (m_pFixedBone->Get_CurrentKeyFrameIndex() == m_iCurFixedBoneKeyFrameIndex)
				m_fFixedBoneHoldTime += (_float)_dDeltaTime * m_fPlaySpeed * m_pCurAnim->Get_PalySpeed();
			else
				m_fFixedBoneHoldTime = 0.f;

			if (1.f < m_fFixedBoneHoldTime)
				return 0;

			vector<KEYFRAME*> KeyFrames = m_pFixedBone->Get_KeyFrames();
			m_iCurFixedBoneKeyFrameIndex = m_pFixedBone->Get_CurrentKeyFrameIndex();
			m_iPreFixedBoneKeyFrameIndex = m_iCurFixedBoneKeyFrameIndex - 1;

			_vector svPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
			_vector svRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
			_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);
			_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

			_vector svPreVelocity = XMVectorZero();
			_vector svVelocity = XMVectorZero();

			_vector svPreQuaternian = XMVectorZero();
			_vector svQuaternian = XMVectorZero();

			if (0 == m_iCurFixedBoneKeyFrameIndex)
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
				if (m_iPreFixedBoneKeyFrameIndex >= 0)
				{
					svPreVelocity = XMLoadFloat3(&KeyFrames[m_iPreFixedBoneKeyFrameIndex]->vPosition);
					svPreQuaternian = XMLoadFloat4(&KeyFrames[m_iPreFixedBoneKeyFrameIndex]->vRotation);
				}
				svVelocity = XMLoadFloat3(&KeyFrames[m_iCurFixedBoneKeyFrameIndex]->vPosition);
				svQuaternian = XMLoadFloat4(&KeyFrames[m_iCurFixedBoneKeyFrameIndex]->vRotation);

				svVelocity -= svPreVelocity;
				svQuaternian -= svPreQuaternian;
			}

			//svVelocity *= _dDeltaTime;
			//svQuaternian = XMVector4Transform(svQuaternian, XMLoadFloat4x4(&m_matPivot));

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
			vRotation = { vEuler.x, vEuler.z, vEuler.y };


			m_pTransform->Rotation_Axis(svRight, XMConvertToRadians(vRotation.x) * (_float)_dDeltaTime * m_fRotSpeed);
			m_pTransform->Rotation_Axis(svUp, XMConvertToRadians(vRotation.y) * (_float)_dDeltaTime * m_fRotSpeed);
			m_pTransform->Rotation_Axis(svLook, XMConvertToRadians(vRotation.z) * (_float)_dDeltaTime * m_fRotSpeed);

			svVelocity = XMLoadFloat3(&vBonePosition);
			svVelocity = XMVector4Transform(svVelocity, XMLoadFloat4x4(&m_matPivot) * m_pTransform->Get_PivotMatrix());
			
			_vector svScale, svRot, svPos;
			XMMatrixDecompose(&svScale, &svRot, &svPos, m_pTransform->Get_WorldMatrix());
			_matrix smatRotation = XMMatrixRotationQuaternion(svRot);
			svVelocity = XMVector4Transform(svVelocity, smatRotation);

			svVelocity *= m_fMoveSpeed;
			m_pTransform->Add_Velocity(svVelocity * (_float)_dDeltaTime);
		}
	}

	return _int();
}

void CAnimationController::Reset_Animation()
{
	m_pCurAnim->Reset_Animation();
}

void CAnimationController::Render_Debug()
{

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

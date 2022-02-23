#include "..\public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(char * pName, _double Duration, _double PlaySpeed)
{
	strcpy_s(m_szName, pName);
	m_Duration = Duration;
	m_PlaySpeed = PlaySpeed;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CChannel * pChannel)
{
	m_Channels.push_back(pChannel);

	return S_OK;
}

HRESULT CAnimation::Update_TransformationMatrix(_double TimeDelta)
{
	m_TrackPositionAcc += m_PlaySpeed * TimeDelta;
	if (m_TrackPositionAcc >= m_Duration)
	{
		m_isFinished = true;
		m_TrackPositionAcc = 0.0;
	}
	else
		m_isFinished = false;
	
	_uint		iNumChannels = (_uint)m_Channels.size();

	for (_uint i = 0; i < iNumChannels; ++i)
	{
		vector<KEYFRAME*>		KeyFrames = m_Channels[i]->Get_KeyFrames();

		_uint	iCurrentKeyFrameIndex = m_Channels[i]->Get_CurrentKeyFrameIndex();

		_vector		vScale, vRotation, vPosition;
		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();

		if (true == m_isFinished)
		{
			iCurrentKeyFrameIndex = 0;
			m_Channels[i]->Set_CurrentKeyFrameIndex(0);
		}

		_uint		iNumKeyFrame = (_uint)KeyFrames.size();

		/* �ϳ��� Ű�����Ӥ� ���¸� ������ �ȴ�. */
		if (m_TrackPositionAcc <= KeyFrames[0]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[0]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[0]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[0]->vPosition);		
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		/* �ϳ��� Ű�����Ӥ� ���¸� ������ �ȴ�. */
		else if (m_TrackPositionAcc > KeyFrames[iNumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[iNumKeyFrame - 1]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		/* Ư�� Ű�����Ӱ� Ű��=���� ���̿� �ִ�.  */
		else
		{
			if (m_TrackPositionAcc >= KeyFrames[iCurrentKeyFrameIndex + 1]->Time)
				m_Channels[i]->Set_CurrentKeyFrameIndex(++iCurrentKeyFrameIndex);

			_float		fRatio = (_float)(m_TrackPositionAcc - KeyFrames[iCurrentKeyFrameIndex]->Time) / 
				(_float)(KeyFrames[iCurrentKeyFrameIndex + 1]->Time - KeyFrames[iCurrentKeyFrameIndex]->Time);

			_vector		vSourScale, vSourRotation, vSourPosition;
			_vector		vDestScale, vDestRotation, vDestPosition;

			vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrameIndex]->vScale);
			vSourRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrameIndex]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrameIndex]->vPosition);

			vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrameIndex + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrameIndex + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrameIndex + 1]->vPosition);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);			
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			vPosition = XMVectorSetW(vPosition, 1.f);		
		}	
		_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		m_Channels[i]->Set_TransformationMatrix(TransformationMatrix);
	}

	return S_OK;
}

CAnimation * CAnimation::Create(char * pName, _double Duration, _double PlaySpeed)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pName, Duration, PlaySpeed)))
	{
		MSGBOX("Failed to Creating CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}

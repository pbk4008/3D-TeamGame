#include "..\public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_Duration(rhs.m_Duration)
	, m_TrackPositionAcc(rhs.m_TrackPositionAcc)
	, m_isFinished(rhs.m_isFinished)
	, m_PlaySpeed(rhs.m_PlaySpeed)
	, m_iIndex(rhs.m_iIndex)
	, m_iCurrentKeyFrameIndex(rhs.m_iCurrentKeyFrameIndex)
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& pPrototypeChannel : rhs.m_Channels)
		m_Channels.push_back(pPrototypeChannel->Clone());
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

HRESULT CAnimation::Update_TransformationMatrix(_double TimeDelta,const _bool _isLoop)
{
	if (!m_isFinished)
		m_TrackPositionAcc += m_PlaySpeed * TimeDelta;

	if (m_TrackPositionAcc >= m_Duration)
	{
		m_isFinished = true;
		if (_isLoop)
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
			if (_isLoop)
			{
				iCurrentKeyFrameIndex = 0;
				m_Channels[i]->Set_CurrentKeyFrameIndex(0);
			}
		}

		_uint		iNumKeyFrame = (_uint)KeyFrames.size();

		/* 하나의 키프에ㅣㅁ 상태를 가지면 된다. */
		if (m_TrackPositionAcc <= KeyFrames[0]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[0]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[0]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[0]->vPosition);		
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		/* 하나의 키프에ㅣㅁ 상태를 가지면 된다. */
		else if (m_TrackPositionAcc > KeyFrames[iNumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[iNumKeyFrame - 1]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		/* 특정 키프레임과 키프=레임 사이에 있다.  */
		else
		{
			while (m_TrackPositionAcc >= KeyFrames[iCurrentKeyFrameIndex + 1]->Time)
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
		m_Channels[i]->Set_AnimInterPolation(vScale, vRotation, vPosition);
	}

	return S_OK;
}

CChannel* CAnimation::Get_Channel(const char* pChannelName) const
{
	for (auto& pChannel : m_Channels)
	{
		const char* pName = pChannel->Get_Name();

		if (!strcmp(pChannelName, pName))
			return pChannel;
	}

	return nullptr;
}

CSaveManager::ANIMDATA& CAnimation::SetSaveAnimData()
{
	CSaveManager::ANIMDATA pData;

	pData.dDuration = m_Duration;
	pData.dPlaySpeed = m_PlaySpeed;
	pData.iAnimIndex = m_iIndex;
	pData.iAnimNameSize = (_uint)strlen(m_szName);
	strcpy_s(pData.szAnimName, m_szName);

	_uint iChannelCnt = (_uint)m_Channels.size();
	pData.iChannelCnt = iChannelCnt;
	pData.pChannelData = new CSaveManager::CHANNELDATA[iChannelCnt];
	for (_uint i = 0; i < iChannelCnt; i++)
	{
		pData.pChannelData[i].iChannelNameSize = (_uint)strlen(m_Channels[i]->Get_Name());
		strcpy_s(pData.pChannelData[i].szChannelName, m_Channels[i]->Get_Name());
		vector<KEYFRAME*> vecKeyFame = m_Channels[i]->Get_KeyFrames();
		_uint iKeyFrameCnt=(_uint)vecKeyFame.size();
		pData.pChannelData[i].iKeyFrameCnt = iKeyFrameCnt;
		pData.pChannelData[i].pKeyFrame = new KEYFRAME[iKeyFrameCnt];
		for (_uint j = 0; j < iKeyFrameCnt; j++)
		{
			pData.pChannelData[i].pKeyFrame[j].Time = vecKeyFame[j]->Time;
			pData.pChannelData[i].pKeyFrame[j].vPosition = vecKeyFame[j]->vPosition;
			pData.pChannelData[i].pKeyFrame[j].vRotation = vecKeyFame[j]->vRotation;
			pData.pChannelData[i].pKeyFrame[j].vScale = vecKeyFame[j]->vScale;
		}
	}
	return pData;
}

void CAnimation::Reset_Animation()
{
	m_isFinished = false;
	m_TrackPositionAcc = 0.0;

	for (auto& pChannel : m_Channels)
		pChannel->Set_CurrentKeyFrameIndex(0);
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

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}

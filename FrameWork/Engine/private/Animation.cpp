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
	, m_iCurKeyFrameIndex(rhs.m_iCurKeyFrameIndex)
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& pPrototypeChannel : rhs.m_Channels)
		m_Channels.push_back(pPrototypeChannel->Clone());
}

HRESULT CAnimation::NativeConstruct(char * pName, _double Duration, _double PlaySpeed, const _int _iIndex)
{
	strcpy_s(m_szName, pName);
	m_Duration = Duration;
	m_PlaySpeed = PlaySpeed;
	m_iIndex = _iIndex;

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
		m_isFinished = true;
	else
		m_isFinished = false;
	
	_uint		iNumChannels = (_uint)m_Channels.size();

	for (_uint i = 0; i < iNumChannels; ++i)
	{
		vector<KEYFRAME*>		KeyFrames = m_Channels[i]->Get_KeyFrames();

		_uint	iCurChannelKeyFrameIndex = m_Channels[i]->Get_CurrentKeyFrameIndex();

		_vector		vScale, vRotation, vPosition;
		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();

		_uint		iNumKeyFrame = (_uint)KeyFrames.size();

		if (m_TrackPositionAcc <= KeyFrames[0]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[0]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[0]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[0]->vPosition);		
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		else if (m_TrackPositionAcc > KeyFrames[iNumKeyFrame - 1]->Time)
		{
			vScale = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vScale);
			vRotation = XMLoadFloat4(&KeyFrames[iNumKeyFrame - 1]->vRotation);
			vPosition = XMLoadFloat3(&KeyFrames[iNumKeyFrame - 1]->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		/* 특정 키프레임과 키프레임 사이에 있다.  */
		else
		{
			while (m_TrackPositionAcc >= KeyFrames[iCurChannelKeyFrameIndex + 1]->Time)
			{
				m_Channels[i]->Set_CurrentKeyFrameIndex(++iCurChannelKeyFrameIndex);
				if (iCurChannelKeyFrameIndex > m_iCurKeyFrameIndex)
					m_iCurKeyFrameIndex = iCurChannelKeyFrameIndex;
			}

			_float		fRatio = (_float)(m_TrackPositionAcc - KeyFrames[iCurChannelKeyFrameIndex]->Time) / 
				(_float)(KeyFrames[iCurChannelKeyFrameIndex + 1]->Time - KeyFrames[iCurChannelKeyFrameIndex]->Time);

			_vector		vSourScale, vSourRotation, vSourPosition;
			_vector		vDestScale, vDestRotation, vDestPosition;

			vSourScale = XMLoadFloat3(&KeyFrames[iCurChannelKeyFrameIndex]->vScale);
			vSourRotation = XMLoadFloat4(&KeyFrames[iCurChannelKeyFrameIndex]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurChannelKeyFrameIndex]->vPosition);

			vDestScale = XMLoadFloat3(&KeyFrames[iCurChannelKeyFrameIndex + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurChannelKeyFrameIndex + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurChannelKeyFrameIndex + 1]->vPosition);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);			
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			vPosition = XMVectorSetW(vPosition, 1.f);		
		}	
		_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		m_Channels[i]->Set_TransformationMatrix(TransformationMatrix);
		m_Channels[i]->Set_AnimInterPolation(vScale, vRotation, vPosition);

		if (m_isFinished)
		{
			if (_isLoop)
			{
				iCurChannelKeyFrameIndex = 0;
				m_Channels[i]->Set_CurrentKeyFrameIndex(0);
			}
		}
	}

	if (m_isFinished)
	{
		m_TrackPositionAcc = 0.0;
		m_iCurKeyFrameIndex = 0;
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

const _uint CAnimation::Get_MaxKeyFrameIndex() const
{
	return m_iMaxKeyFrameIndex;
}

const _uint CAnimation::Get_CurrentKeyFrameIndex()
{
	return m_iCurKeyFrameIndex;
}

void CAnimation::Set_MaxKeyFrameIndex(const _uint _iMaxKeyFrameIndex)
{
	m_iMaxKeyFrameIndex = _iMaxKeyFrameIndex;
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
	m_iCurKeyFrameIndex = 0;

	for (auto& pChannel : m_Channels)
		pChannel->Set_CurrentKeyFrameIndex(0);
}

CAnimation * CAnimation::Create(char * pName, _double Duration, _double PlaySpeed, const _int _iIndex)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pName, Duration, PlaySpeed, _iIndex)))
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

#include "..\public\Channel.h"



CChannel::CChannel()
{
}

CChannel::CChannel(const CChannel& rhs)
	: m_TransformationMatrix(rhs.m_TransformationMatrix)
	, m_iCurrentKeyFrameIndex(rhs.m_iCurrentKeyFrameIndex)
	, m_KeyFrames(rhs.m_KeyFrames)
	, m_isClone(true)
	, m_isRoot(rhs.m_isRoot)
	, m_tAnimInterPolation(rhs.m_tAnimInterPolation)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CChannel::Set_AnimInterPolation(_fvector _vScale, _fvector _vRotation, _fvector _vPosition)
{
	XMStoreFloat4(&m_tAnimInterPolation.vScale, _vScale);
	XMStoreFloat4(&m_tAnimInterPolation.vRotation, _vRotation);
	XMStoreFloat4(&m_tAnimInterPolation.vPosition, _vPosition);
}

HRESULT CChannel::NativeConstruct(const char* pName)
{
	strcpy_s(m_szName, pName);
	if (!strcmp(m_szName, "root"))
		m_isRoot = true;

	return S_OK;
}

HRESULT CChannel::Add_KeyFrame(KEYFRAME * pKeyFrame)
{
	m_KeyFrames.push_back(pKeyFrame);

	return S_OK;
}

CChannel * CChannel::Create(const char* pName)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct(pName)))
	{
		MSGBOX("Failed to Creating CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Clone()
{
	return new CChannel(*this);
}

void CChannel::Free()
{
	if (!m_isClone)
	{
		for (auto& pKeyFrame : m_KeyFrames)
			Safe_Delete(pKeyFrame);
	}

	m_KeyFrames.clear();
}

#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;
public:
	vector<KEYFRAME*> Get_KeyFrames() {
		return m_KeyFrames;
	}

	_uint Get_CurrentKeyFrameIndex() const {
		return m_iCurrentKeyFrameIndex;
	}

	_matrix Get_TransformMatrix() const {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}

	void Set_CurrentKeyFrameIndex(_uint KeyFrameIndex) {
		m_iCurrentKeyFrameIndex = KeyFrameIndex;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}
public:
	HRESULT NativeConstruct(const char* pName);
	HRESULT Add_KeyFrame(KEYFRAME* pKeyFrame);

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iCurrentKeyFrameIndex = 0;
	_float4x4				m_TransformationMatrix;


	/* ���� ä��(��)�� ǥ���ؾ��� Ű�����ӿ� ���� �������(������, ȸ��, �̵�)���� �����Ѵ�. */
	vector<KEYFRAME*>			m_KeyFrames;
	typedef vector<KEYFRAME*>	KEYFRAMES;

public:
	static CChannel* Create(const char* pName);
	virtual void Free() override;
};

END
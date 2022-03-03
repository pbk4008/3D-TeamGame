#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	typedef struct tagAnimInterPol
	{
		_float4 vPosition;
		_float4 vScale;
		_float4 vRotation;
	}ANIMINTERPOL;
public:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;
public:
	vector<KEYFRAME*> Get_KeyFrames() {
		return m_KeyFrames;
	}
	const char* Get_Name() const {
		return m_szName;
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
	void Set_AnimInterPolation(_fvector _vScale, _fvector _vRotation, _fvector _vPosition);
	void Set_RootAnimInterPolation(_fvector _vScale, _fvector _vRotation, _fvector _vPosition);
public:
	ANIMINTERPOL& getAnimInterPolation() { return m_tAnimInterPolation; }
	ANIMINTERPOL& getRootAnimInterPolation() { return m_tRootAnimInterPolation; }
public:
	HRESULT NativeConstruct(const char* pName);
	HRESULT Add_KeyFrame(KEYFRAME* pKeyFrame);

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iCurrentKeyFrameIndex = 0;
	_float4x4				m_TransformationMatrix;
	_bool					m_isRoot = false;

	_bool m_isClone = false;

	/* 현재 채널(뼈)이 표현해야할 키프레임에 따른 상태행렬(스케일, 회전, 이동)들을 보관한다. */
	vector<KEYFRAME*>			m_KeyFrames;
	typedef vector<KEYFRAME*>	KEYFRAMES;
private:
	ANIMINTERPOL		m_tAnimInterPolation;
	ANIMINTERPOL		m_tRootAnimInterPolation;

public:
	static CChannel* Create(const char* pName);
	CChannel* Clone();
	virtual void Free() override;
};

END
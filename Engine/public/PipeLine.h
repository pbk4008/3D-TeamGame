#pragma once

/* ���� �� �����Ӥ��� ���� �� �ִ� ��, ������ȯ������� �����ϳ�. */

#include "Base.h"

class CPipeLine : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATEMATRIX { D3DTS_VIEW, D3DTS_PROJECTION, D3DTS_END };
public:
	CPipeLine();
	virtual ~CPipeLine() = default;
public:
	_fmatrix Get_Transform(TRANSFORMSTATEMATRIX eType) {
		return XMLoadFloat4x4(&m_TransformMatrix[eType]);
	}

	_fvector Get_CamPosition() {
		return XMLoadFloat4(&m_vWorldCamPosition);
	}

	void Set_Transform(TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix[eType], TransformMatrix);
	}

	void Update_PipeLine();
private:
	_float4x4			m_TransformMatrix[D3DTS_END];
	_float4				m_vWorldCamPosition;

public:
	virtual void Free() override;
};


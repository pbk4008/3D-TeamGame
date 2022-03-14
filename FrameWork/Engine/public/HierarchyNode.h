#pragma once

#include "Base.h"
#include "Channel.h"

/* 뼈하나의 정보를 표현하는 노드다. */

BEGIN(Engine)

class ENGINE_DLL CHierarchyNode : public CBase
{
private:
	explicit CHierarchyNode()= default;
	virtual ~CHierarchyNode() = default;

public:
	_uint	Get_Depth() const { return m_iDepth; }
	const CHierarchyNode* Get_Parent() const { return m_pParent; }
	const char* Get_Name() const { return m_szBoneName; }

	_fmatrix Get_CombinedMatrix() const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }
	_fmatrix Get_TransformMatrix() const {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}
	_fmatrix Get_OffsetMatrix() const { return XMLoadFloat4x4(&m_OffsetMatrix); }

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) { XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix); }


public:
	void Reserve_Channel(_uint iNumAnimation) { m_Channels.resize(iNumAnimation); }
	void Add_Channel(_uint iAnimationIndex, class CChannel* pChannel) 
	{
		m_Channels[iAnimationIndex] = pChannel;
		Safe_AddRef(pChannel);
	}

public:
	HRESULT NativeConstruct(char* pBoneName, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent);
	HRESULT NativeConstruct(char* pBoneName, _fmatrix OffsetMatrix, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent);
	void Update_CombinedTransformationMatrix(_uint iAnimationIndex=0);
	void Update_CombinedTransformationMatrix(const _uint _iAnimIndex, const _bool _isRootMotion, const ERootOption _eRootOption);
private:
	/* 노드의 이름과 뼈의 이름은 1:1로 매칭된다. */
	char						m_szBoneName[MAX_PATH] = "";
	CHierarchyNode*		m_pParent = nullptr;
	_uint						m_iDepth = 0;

	_float4x4			m_OffsetMatrix;
	_float4x4			m_TransformationMatrix;
	_float4x4			m_CombinedTransformationMatrix;

	/* 정점을 그리기위한 최종 행렬은. */
	/* m_OffsetMatrix * m_CombinedTransformationMatrix(m_TransformationMatrix * 부모`s m_CombinedTransformationMatrix) * 객체`s 월드 */


	vector<class CChannel*>			m_Channels;

public:
	static CHierarchyNode* Create(char* pBoneName, _fmatrix TransformationMatrix, _uint iDepth = 0, CHierarchyNode* pParent = nullptr);
	static CHierarchyNode* Create(char* pBoneName, _fmatrix OffsetMatrix, _fmatrix TransformationMatrix, _uint iDepth, CHierarchyNode* pParent);
	virtual void Free() override;
};

END
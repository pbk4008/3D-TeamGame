#include "pch.h"
#include "TrailEffect.h"

CTrailEffect::CTrailEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CTrailEffect::CTrailEffect(const CTrailEffect& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTrailEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrailEffect::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (_pArg)
	{
		//memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_tDesc = *static_cast<DESC*>(_pArg);
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	return S_OK;
}

_int CTrailEffect::Tick(_double _dDeltaTime)
{

	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CTrailEffect::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CTrailEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTrailEffect::Set_IsRender(const _bool _isRender)
{
	m_isRender = _isRender;
}

void CTrailEffect::Set_Length(const _float _fLength)
{
	m_tDesc.fLength = _fLength;
}

void CTrailEffect::Set_Texture(const wstring& _wstrTextureTag)
{
	if (!m_pTexture)
		return;
	m_pTexture->Change_Texture(_wstrTextureTag);
}

void CTrailEffect::Set_PivotMatrix(const _fmatrix& _smatPivot)
{
	XMStoreFloat4x4(&m_tDesc.matPivot, _smatPivot);
}

void CTrailEffect::Record_Points(const _double& _dDeltaTIme)
{
	while (32 <= m_listPoints.size())
	{
		m_listPoints.pop_front();
	}

	m_fAccTime += (_float)_dDeltaTIme;

	if (0.01f < m_fAccTime)
	{
		_float3 vStartPos = { 0.f, 0.f, 0.f };
		_float3 vEndPos = { 0.f, 0.f, -m_tDesc.fLength };

		_vector svStartPos = XMLoadFloat3(&vStartPos);
		_vector svEndPos = XMLoadFloat3(&vEndPos);

		_matrix smatOwnerWorld = m_tDesc.pOwnerTransform->Get_WorldMatrix();
		svStartPos = XMVector3TransformCoord(svStartPos, XMLoadFloat4x4(&m_tDesc.matPivot) * smatOwnerWorld);
		svEndPos = XMVector3TransformCoord(svEndPos, XMLoadFloat4x4(&m_tDesc.matPivot) * smatOwnerWorld);

		XMStoreFloat3(&vStartPos, svStartPos);
		XMStoreFloat3(&vEndPos, svEndPos);
		m_listPoints.emplace_back(vStartPos, vEndPos);

		m_fAccTime = 0.f;
	}
}

void CTrailEffect::Clear_Points()
{
	m_listPoints.clear();
}

void CTrailEffect::CatmullRom()
{
	if (4 > m_listPoints.size())
		return;

	auto iter = m_listPoints.begin();
	auto iter_end = m_listPoints.end();
	const _uint iListSize = (_uint)m_listPoints.size();
	_vector* svStartPoints = new _vector[iListSize];
	_vector* svEndPoints = new _vector[iListSize];

	_uint iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		svStartPoints[iIndex] = XMVectorSetW(XMLoadFloat3(&iter->first), 1.f);
		svEndPoints[iIndex] = XMVectorSetW(XMLoadFloat3(&iter->second), 1.f);
		++iIndex;
	}

	_uint iNum = 10;
	for (_uint i = 0; i < iListSize - 3; ++i)
	{
		for (_uint j = 1; j < iNum; ++j)
		{
			_vector svStartPoint, svEndPoint;
			svStartPoint = XMVectorCatmullRom(svStartPoints[i], svStartPoints[i + 1], svStartPoints[i + 2], svStartPoints[i + 3], j / (_float)iNum);
			svEndPoint = XMVectorCatmullRom(svEndPoints[i], svEndPoints[i + 1], svEndPoints[i + 2], svEndPoints[i + 3], j / (_float)iNum);

			_float3 vStartPoint, vEndPoint;
			XMStoreFloat3(&vStartPoint, svStartPoint);
			XMStoreFloat3(&vEndPoint, svEndPoint);
			m_listCurved.emplace_back(vStartPoint, vEndPoint);
		}
	}

	//_float3 vFirstStartPoint, vFirstEndPoint;
	//XMStoreFloat3(&vFirstStartPoint, svStartPoints[0]);
	//XMStoreFloat3(&vFirstEndPoint, svEndPoints[0]);
	//_float3 vLastStartPoint, vLastEndPoint;
	//XMStoreFloat3(&vLastStartPoint, svStartPoints[iListSize - 1]);
	//XMStoreFloat3(&vLastEndPoint, svEndPoints[iListSize - 1]);

	//m_listCurved.emplace_front(vFirstStartPoint, vFirstEndPoint);
	//m_listCurved.emplace_back(vLastStartPoint, vLastEndPoint);

	Safe_Delete_Array(svStartPoints);
	Safe_Delete_Array(svEndPoints);
}

void CTrailEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}

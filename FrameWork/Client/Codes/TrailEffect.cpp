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
		m_pTargetTransform = static_cast<CTransform*>(_pArg);

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		MSGBOX(L"트레일 이펙트 레디 컴포넌츠 실패");

	XMStoreFloat4x4(&m_matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));

	return S_OK;
}

_int CTrailEffect::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	XMStoreFloat4x4(&m_matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));

	return _int();
}

_int CTrailEffect::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if(m_isRender)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this,1.f);

	return _int();
}

HRESULT CTrailEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (!m_pTexture)
		return S_OK;

	if (4 > m_listPoints.size())
		return S_OK;

#pragma region 스플라인 안먹일 때 (시작지점 + 끝지점 사용)
	//auto iter = m_listPoints.begin();
	//auto iter_end = m_listPoints.end();
	//const _uint iListSIze = (_uint)m_listPoints.size();
	//_float3* vStartPoints = new _float3[iListSIze];
	//_float3* vEndPoints = new _float3[iListSIze];

	//_uint iIndex = 0;
	//for (; iter != iter_end; ++iter)
	//{
	//	vStartPoints[iIndex] = iter->first;
	//	vEndPoints[iIndex] = iter->second;
	//	++iIndex;
	//}

	//m_pVIBuffer->Set_VertexTrail(vStartPoints, vEndPoints, iListSIze);
	//Safe_Delete_Array(vStartPoints);
	//Safe_Delete_Array(vEndPoints);
#pragma endregion
#pragma region 스플라인 먹일 때 (스타트 포지션, 엔드 포지션)
	CatmullRom();

	auto iter = m_listCurved.begin();
	auto iter_end = m_listCurved.end();
	const _uint iListSize = (_uint)m_listCurved.size();
	_float3* vStartPoints = new _float3[iListSize];
	_float3* vEndPoints = new _float3[iListSize];

	_uint iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		vStartPoints[iIndex] = iter->first;
		vEndPoints[iIndex] = iter->second;
		++iIndex;
	}
	m_pVIBuffer->Set_VertexTrail(vStartPoints, vEndPoints, iListSize);
	Safe_Delete_Array(vStartPoints);
	Safe_Delete_Array(vEndPoints);
#pragma endregion

	m_Frametime += 0.01f;

	if (m_Frametime >= 1000.0f)
	{
		m_Frametime = 0.0f;
	}

	_matrix world, view, proj;

	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("NoiseFire ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4)))) MSGBOX("NoiseFire ConstBuffer Projmatrix Not Apply");

	_float3 scrollspeeds = _float3(1.5f, 2.6f, 2.8f); //_float3(1.3f, 2.1f, 2.3f);
	_float3 scales = _float3(1.0f, 2.0f, 3.0f);//_float3(1.0f, 2.0f, 3.0f);
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_frametime", &m_Frametime, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_scrollspeeds", &scrollspeeds, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_scales", &scales, sizeof(_float3)))) MSGBOX("NoiseFire ConstBuffer NosieBuffer Not Apply");

	_float2 distoriton1 = _float2(0.1f, 0.5f);
	_float2 distoriton2 = _float2(0.1f, 0.6f);
	_float2 distoriton3 = _float2(0.1f, 0.4f);
	_float distoritonScale = 0.8f;
	_float distoritonBiase = 0.2f;

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distortion1", &distoriton1, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distortion2", &distoriton2, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distortion3", &distoriton3, sizeof(_float2)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distortionscale", &distoritonScale, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distortionbias", &distoritonBiase, sizeof(_float)))) MSGBOX("NoiseFire ConstBuffer Distorition buffer Not Apply");

	// Texture
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture)))		MSGBOX("NoiseFire ConstBuffer FireTexture Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DistortionTex", m_pTexture1)))	MSGBOX("NoiseFire ConstBuffer NoiseTexture Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DistorionMaskTex", m_pTexture2)))	MSGBOX("NoiseFire ConstBuffer AlphaTexture Not Apply");

	m_pVIBuffer->Render_Curve(1);

	m_listCurved.clear();

	return S_OK;
}

HRESULT CTrailEffect::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Trail", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
		MSGBOX(L"트레일 버퍼 못만듬");

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"TrailBase");

	m_pTexture1 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	m_pTexture1->Change_Texture(L"DistortionBase");

	m_pTexture2 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	m_pTexture2->Change_Texture(L"DistortionMask");

	return S_OK;
}

void CTrailEffect::Set_IsRender(const _bool _isRender)
{
	m_isRender = _isRender;
}

void CTrailEffect::Record_Points(const _double& _dDeltaTIme)
{
	while (30 <= m_listPoints.size())
	{
		m_listPoints.pop_front();
	}

	m_fAccTime += (_float)_dDeltaTIme;

	if (0.01f < m_fAccTime)
	{
		_float3 vStartPos = { 0.f, 0.f, 0.f };
		_float3 vEndPos = { 0.f, 0.f, -1.f };

		_vector svStartPos = XMLoadFloat3(&vStartPos);
		_vector svEndPos = XMLoadFloat3(&vEndPos);

		_matrix smatTargetWorld = m_pTargetTransform->Get_WorldMatrix();
		svStartPos = XMVector3TransformCoord(svStartPos, XMLoadFloat4x4(&m_matPivot) * smatTargetWorld);
		svEndPos = XMVector3TransformCoord(svEndPos, XMLoadFloat4x4(&m_matPivot) * smatTargetWorld);

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

CTrailEffect* CTrailEffect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CTrailEffect* pInstance = new CTrailEffect(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrailEffect Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrailEffect::Clone(_uint _iSceneID, void* _pArg)
{
	CTrailEffect* pInstance = new CTrailEffect(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CTrailEffect Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrailEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pTexture1);
	Safe_Release(m_pTexture2);
	Safe_Release(m_pVIBuffer);
}

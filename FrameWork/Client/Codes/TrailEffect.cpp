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
	//if (m_pTargetTransform)
	//{
	//	Record_Points(_dDeltaTime);
	//}

	return _int();
}

_int CTrailEffect::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if(m_isRender)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CTrailEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (!m_pTexture)
		return S_OK;
	//if (6 > m_listPoints.size()) // 리스트에 pair 가 아니라 pos, dir를 하나로 쭉 이어 붙였을 때
	//	return S_OK;
	//if (3 > m_listPoints.size()) // 스플라인 처리를 안해줄때
	//	return S_OK;
	if (4 > m_listPoints.size())
		return S_OK;

#pragma region 스플라인 안먹일 때
	auto iter = m_listPoints.begin();
	auto iter_end = m_listPoints.end();
	//_float3* vPoints = new _float3[m_listPoints.size() / 2];
	//_float3* vDirs = new _float3[m_listPoints.size() / 2];
	_float3* vPoints = new _float3[m_listPoints.size()];
	_float3* vDirs = new _float3[m_listPoints.size()];

	_uint iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		//vPoints[iIndex] = (*iter);
		//++iter;
		//vDirs[iIndex] = (*iter);
		//++iIndex;
		vPoints[iIndex] = iter->first;
		vDirs[iIndex] = iter->second;
		++iIndex;
	}

	m_pVIBuffer->Set_VertexTrail(vPoints, vDirs, m_fLength, (DWORD)m_listPoints.size());
#pragma endregion


	//CatmullRom();

	//auto iter = m_listCurved.begin();
	//auto iter_end = m_listCurved.end();
	//_float3* vPoints = new _float3[m_listCurved.size()];
	//_float3* vDirs = new _float3[m_listCurved.size()];

	//_uint iIndex = 0;
	//for (; iter != iter_end; ++iter)
	//{
	//	vPoints[iIndex] = iter->first;
	//	vDirs[iIndex] = iter->second;
	//	++iIndex;
	//}
	//m_pVIBuffer->Set_VertexTrail(vPoints, vDirs, m_fLength, m_listCurved.size());

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

	/*_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(XMMatrixIdentity());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));
	m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);
	m_pVIBuffer->SetUp_TextureOnShader("g_DistortionTex", m_pTexture1);
	m_pVIBuffer->SetUp_TextureOnShader("g_DistorionMaskTex", m_pTexture2);*/

	m_pVIBuffer->Render_Curve(1);

	m_listCurved.clear();
	Safe_Delete_Array(vPoints);
	Safe_Delete_Array(vDirs);

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
		//m_listPoints.pop_front();
	}

	m_fAccTime += (_float)_dDeltaTIme;

	//if (0.02f < m_fAccTime)
	//{
		_float3 vPoints[2];

		vPoints[0] = _float3(0.f, 0.f, 0.f);
		vPoints[1] = _float3(0.f, 0.f, -0.4f);

		_vector svPos, svDir;
		svDir = XMLoadFloat3(&vPoints[1]) - XMLoadFloat3(&vPoints[0]);
		m_fLength = XMVectorGetX(XMVector3Length(svDir));

		svDir = XMVector3Normalize(svDir);
		svPos = svDir * m_fLength;

		_matrix smatTargetWorld = m_pTargetTransform->Get_WorldMatrix();
		svPos = XMVector3TransformCoord(svPos, XMLoadFloat4x4(&m_matPivot) * smatTargetWorld);
		svDir = XMVector3TransformNormal(svDir, XMLoadFloat4x4(&m_matPivot) * smatTargetWorld);

		_float3 vPos, vDir;
		XMStoreFloat3(&vPos, svPos);
		XMStoreFloat3(&vDir, svDir);
		//m_listPoints.emplace_back(vPos);
		//m_listPoints.emplace_back(vDir);
		m_listPoints.emplace_back(vPos, vDir);

		m_fAccTime = 0.f;
	//}
}

void CTrailEffect::Clear_Points()
{
	m_listPoints.clear();
}

void CTrailEffect::CatmullRom()
{
	if (4 > m_listPoints.size())
		return;

	//auto iter = m_listPoints.begin();
	//auto iter0 = iter;
	//auto iter1 = ++iter;
	//auto iter2 = ++iter;
	//auto iter3 = ++iter;
	//auto iter_end = m_listPoints.end();

	//for (; iter3 != iter_end;)
	//{
	//	_vector svPos, svDir;
	//	svPos = XMVectorCatmullRom(XMLoadFloat3(&iter0->first), XMLoadFloat3(&iter1->first), XMLoadFloat3(&iter2->first), XMLoadFloat3(&iter3->first), 0.5f);
	//	svDir = XMVectorCatmullRom(XMLoadFloat3(&iter0->second), XMLoadFloat3(&iter1->second), XMLoadFloat3(&iter2->second), XMLoadFloat3(&iter3->second), 0.5f);

	//	_float3 vPos, vDir;
	//	XMStoreFloat3(&vPos, svPos);
	//	XMStoreFloat3(&vDir, svDir);

	//	m_listPoints.insert(iter2, { vPos, vDir });

	//	iter0 = iter1;
	//	iter1 = iter2;
	//	iter2 = iter3;
	//	++iter3;
	//}


	auto iter = m_listPoints.begin();
	auto iter_end = m_listPoints.end();
	_vector* vPoints = new _vector[m_listPoints.size()];
	_vector* vDirs = new _vector[m_listPoints.size()];
	_uint iPointsSize = (_uint)m_listPoints.size();

	_uint iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		vPoints[iIndex] = XMVectorSetW(XMLoadFloat3(&iter->first), 1.f);
		vDirs[iIndex] = XMLoadFloat3(&iter->second);
		++iIndex;
	}

	_uint iNum = 10;
	for (_uint i = 0; i < iPointsSize - 3; ++i)
	{
		for (_uint j = 0; j <= iNum; ++j)
		{
			_vector svPos, svDir;
			//svPos = XMVectorCatmullRom(XMLoadFloat3(&vPoints[i]), XMLoadFloat3(&vPoints[i + 1]), XMLoadFloat3(&vPoints[i + 2]), XMLoadFloat3(&vPoints[i + 3]), j / 4.f);
			svPos = XMVectorCatmullRom(vPoints[i], vPoints[i + 1], vPoints[i + 2], vPoints[i + 3], j / (_float)iNum);
			//svDir = XMVectorCatmullRom(XMLoadFloat3(&vDirs[i]), XMLoadFloat3(&vDirs[i + 1]), XMLoadFloat3(&vDirs[i + 2]), XMLoadFloat3(&vDirs[i + 3]), j / 4.f);
			svDir = XMVectorCatmullRom(vDirs[i], vDirs[i + 1], vDirs[i + 2], vDirs[i + 3], j / (_float)iNum);

			_float3 vPos, vDir;
			XMStoreFloat3(&vPos, svPos);
			XMStoreFloat3(&vDir, svDir);
			m_listCurved.emplace_back(vPos, vDir);
		}
	}

	Safe_Delete_Array(vPoints);
	Safe_Delete_Array(vDirs);
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

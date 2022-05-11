#include "pch.h"
#include "TrailEffect_Distortion.h"

CTrailEffect_Distortion::CTrailEffect_Distortion(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CTrailEffect(_pDevice, _pDeviceContext)
{
}

CTrailEffect_Distortion::CTrailEffect_Distortion(const CTrailEffect_Distortion& _rhs)
	: CTrailEffect(_rhs)
{
}

HRESULT CTrailEffect_Distortion::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrailEffect_Distortion::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		MSGBOX(L"디스토션 트레일 레디 컴포넌츠 실패");

	return S_OK;
}

_int CTrailEffect_Distortion::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CTrailEffect_Distortion::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;


	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_DYDISTORTION, this);

	return _int();
}

HRESULT CTrailEffect_Distortion::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (!m_pTexture2)
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

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_float4x4))))	MSGBOX("Trail ConstBuffer Worldmatrix Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_float4x4)))) MSGBOX("Trail ConstBuffer Viewmatrix Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_float4x4)))) MSGBOX("Trail ConstBuffer Projmatrix Not Apply");

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DistorionMaskTex", m_pTexture2)))	MSGBOX("Trail ConstBuffer AlphaTexture Not Apply");

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_deltatime", &g_fDeltaTime, sizeof(_float)))) MSGBOX("Trail ConstBuffer Weight Not Apply");

	m_pVIBuffer->Render_Curve(0);

	m_listCurved.clear();

	return S_OK;
}

HRESULT CTrailEffect_Distortion::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Trail_Distortion", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
		MSGBOX(L"트레일 버퍼 못만듬");

	//m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	//m_pTexture->Change_Texture(m_tDesc.wstrTextureTag);

	//m_pTexture1 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	//m_pTexture1->Change_Texture(L"DistortionBase");

	m_pTexture2 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");;
	m_pTexture2->Change_Texture(L"DistortionMask");

	return S_OK;
}

CTrailEffect_Distortion* CTrailEffect_Distortion::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CTrailEffect_Distortion* pInstance = new CTrailEffect_Distortion(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrailEffect_Distortion Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrailEffect_Distortion::Clone(_uint _iSceneID, void* _pArg)
{
	CTrailEffect_Distortion* pInstance = new CTrailEffect_Distortion(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CTrailEffect_Distortion Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrailEffect_Distortion::Free()
{
	__super::Free();

	//Safe_Release(m_pTexture1);
	Safe_Release(m_pTexture2);
}

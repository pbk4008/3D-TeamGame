#include "pch.h"
#include "TrailEffect_Normal.h"

CTrailEffect_Normal::CTrailEffect_Normal(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CTrailEffect(_pDevice, _pDeviceContext)
{
}

CTrailEffect_Normal::CTrailEffect_Normal(const CTrailEffect_Normal& _rhs)
	: CTrailEffect(_rhs)
{
}

HRESULT CTrailEffect_Normal::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrailEffect_Normal::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		MSGBOX(L"노말 트레일 레디 컴포넌츠 실패");

	return S_OK;
}

_int CTrailEffect_Normal::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CTrailEffect_Normal::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTrailEffect_Normal::Render()
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

	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(XMMatrixIdentity());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	_float4 color = _float4(1, 0, 0, 1);
	_float empower = 0.7f;
	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_color", &color, sizeof(_float4));
	m_pVIBuffer->SetUp_ValueOnShader("g_empower", &empower, sizeof(_float));
	m_pVIBuffer->SetUp_ValueOnShader("g_weight", &empower, sizeof(_float));
	m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pVIBuffer->Render_Curve(3);
	m_listCurved.clear();

	return S_OK;
}

HRESULT CTrailEffect_Normal::Ready_Components()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Trail_Normal", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
		MSGBOX(L"트레일 버퍼 못만듬");

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(m_tDesc.wstrTextureTag);

	return S_OK;
}

CTrailEffect_Normal* CTrailEffect_Normal::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CTrailEffect_Normal* pInstance = new CTrailEffect_Normal(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrailEffect_Normal Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrailEffect_Normal::Clone(_uint _iSceneID, void* _pArg)
{
	CTrailEffect_Normal* pInstance = new CTrailEffect_Normal(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CTrailEffect_Normal Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrailEffect_Normal::Free()
{
	__super::Free();
}

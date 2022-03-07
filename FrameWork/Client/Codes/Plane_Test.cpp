#include "pch.h"
#include "Plane_Test.h"

CPlane_Test::CPlane_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CPlane_Test::CPlane_Test(const CPlane_Test& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CPlane_Test::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane_Test::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CPlane_Test::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

_int CPlane_Test::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

#ifdef _DEBUG
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
#endif
	return _int();
}

HRESULT CPlane_Test::Render()
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");

	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CPlane_Test::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"VIBuffer_Plane", L"VIBuffer", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* Com_Texture  */
	wstring TexTag = L"Plane_Texture";
	m_pTexture = (CTexture*)g_pGameInstance->Clone_Component((_uint)SCENEID::SCENE_TEST_JS, L"Plane_Texture", &TexTag);

	return S_OK;
}

CPlane_Test* CPlane_Test::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CPlane_Test* pInstance = new CPlane_Test(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPlane_Test Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlane_Test::Clone(void* _pArg)
{
	CPlane_Test* pInstance = new CPlane_Test(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CPlane_Test Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlane_Test::Free()
{
}

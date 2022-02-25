#include "pch.h"
#include "Plane.h"
#include "GameInstance.h"

CPlane::CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
: CGameObject(_pDevice, _pDeviceContext)
{
}

CPlane::CPlane(const CPlane& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CPlane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CPlane::Tick(_double TimeDelta)
{
	return _int();
}

_int CPlane::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

HRESULT CPlane::Render()
{
	Set_WVPMatrix();

#ifdef _DEBUG

	m_pVIBufferCom->Render(0);

#endif // _DEBUG

	return S_OK;
}

HRESULT CPlane::SetUp_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TAB_MAP, L"Prototype_Component_VIBuffer_Terrain", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CPlane::Set_WVPMatrix()
{

	//_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	//auto vCamPos = pGameInstance->Get_CamPosition();

	//auto matWorldInvers = XMMatrixTranspose(matWorld);
	//auto matViewInvers = XMMatrixTranspose(pGameInstance->Get_Transform(,TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	//auto matProjInvers = XMMatrixTranspose(pGameInstance->Get_Transform(,TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	//m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	//m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	//m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	//m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	//m_pVIBufferCom->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));
}

CPlane* CPlane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CPlane* pInstance = new CPlane(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CPlane!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlane::Clone(void* pArg)
{
	CPlane* pInstance = new CPlane(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CPlane!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlane::Free(void)
{
	Safe_Release(m_pVIBufferCom);
}

#include "pch.h"
#include "Plane.h"
#include "GameInstance.h"
#include "Mouse.h"
#include "NavSphere.h"
#include "GameObject.h"
#include "Observer.h"
#include "Cell.h"
#include "VIBuffer_Triangle.h"

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
	m_pObserver = GET_INSTANCE(CObserver);

	if (CObserver::MODE_NAV == m_pObserver->m_eMode)
	{
		if (3 == m_iPointindex)
			Make_NavigationCell();
		else if(true == m_pObserver->m_bPlanePick)
			Create_NavigationLine();
	}
	RELEASE_INSTANCE(CObserver);

	return _int();
}

_int CPlane::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	Update_CellPos();
	return _int();
}

HRESULT CPlane::Render()
{
	Set_WVPMatrix();

#ifdef _DEBUG
	m_pVIBufferCom->Render(0);
	m_pNavigationCom->Render(L"Camera");
#endif // _DEBUG

	return S_OK;
}

HRESULT CPlane::SetUp_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TAB_STATIC, L"Prototype_Component_VIBuffer_Plane", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Navigation */
	if (FAILED(__super::SetUp_Components(TAB_STATIC, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	/* Com_Texture  */
	wstring TexTag = L"Plane_Texture";
	m_pTexture = (CTexture*)g_pGameInstance->Clone_Component(TAB_STATIC, L"Texture", &TexTag);

	return S_OK;
}

HRESULT CPlane::Create_NavigationLine()
{
	if (true == m_pObserver->m_bNavSpherePick)
	{
		CNavSphere* pSphere = dynamic_cast<CNavSphere*>(m_pObserver->m_pNavSphere);
		m_fPoints[m_iPointindex++] = &(pSphere->m_fPostion);
		m_pObserver->m_bNavSpherePick = false;
		return S_OK;
	}
	else
	{
		CGameObject* pSphere = nullptr;
		if (SUCCEEDED(g_pGameInstance->Add_GameObjectToLayer(TAB_STATIC, L"Layer_NaveSphere", L"Prototype_GameObject_NavSphere", &m_pObserver->m_fPickPos, &pSphere)))
		{
			m_fPoints[m_iPointindex++] = &(dynamic_cast<CNavSphere*>(pSphere)->m_fPostion);
			return S_OK;
		}
		else
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlane::Update_CellPos()
{
	CNavSphere* pSphere = m_pObserver->m_pNavSphere;
	if (nullptr == pSphere)
		return E_FAIL;

	if (FAILED(m_pNavigationCom->Update_Buffer(XMLoadFloat3(&pSphere->m_fPostion))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::Make_NavigationCell(CCell* _pCell/*= nullptr*/)
{
	CCell* pCell = nullptr;

	if (nullptr == _pCell)
	{
		CCW_Sort((*m_fPoints));
		pCell = CCell::Create(m_pDevice, m_pDeviceContext, m_fPoints, m_pNavigationCom->m_Cells.size());
	}
	else
	{
		memcpy(m_fPoints, _pCell->m_vPoint, sizeof(_float3) * CCell::POINT_END);
		CCW_Sort((*m_fPoints));
		pCell = CCell::Create(m_pDevice, m_pDeviceContext, *m_fPoints, m_pNavigationCom->m_Cells.size());
	}

	m_pNavigationCom->m_Cells.push_back(pCell);
	m_iPointindex = 0;

	return S_OK;
}

void CPlane::CCW_Sort(_float3* pPoints)
{
	_int temp = (pPoints[0].x * pPoints[1].z) +
		(pPoints[1].x * pPoints[2].z) +
		(pPoints[2].x * pPoints[0].z);

	temp = temp - (pPoints[0].z * pPoints[1].x) -
		(pPoints[1].z * pPoints[2].x) -
		(pPoints[2].z * pPoints[0].x);

	if (temp > 0)
	{
		_float3 fPointTemp = pPoints[1];
		pPoints[1] = pPoints[2];
		pPoints[2] = fPointTemp;
		CCW_Sort(pPoints);
	}
	else if (temp < 0)
	{
		return;
	}
}

void CPlane::Set_WVPMatrix()
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera");

	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, 0);

	m_pVIBufferCom->SetUp_ValueOnShader("g_vCamPosition", &vCamPos, sizeof(_vector));
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
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);
}

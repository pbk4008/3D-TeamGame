#include "framework.h"
#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround()
	:m_pRcTex(nullptr)
	, m_pTexture(nullptr)
{
}

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pRcTex(nullptr)
	, m_pTexture(nullptr)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CGameObject(rhs)
	, m_pRcTex(rhs.m_pRcTex)
	, m_pTexture(rhs.m_pTexture)
{
	Safe_AddRef(m_pRcTex);
	Safe_AddRef(m_pTexture);
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Texture", L"BackGroundTex", (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"RectBuffer", L"BackGroundBuffer", (CComponent**)&m_pRcTex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Ready_GameObject(pArg)))
		return E_FAIL;

	m_fPos = { 640.f,360.f };
	m_fSize = { 1280.f,720.f };

	_vector vPos = { m_fPos.x - (g_iWinCX >> 1),-m_fPos.y + (g_iWinCY >> 1),1.f,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_fSize.x,m_fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	return S_OK;
}

_int CBackGround::Tick(_double fDeltaTime)
{
	if (FAILED(CGameObject::Tick(fDeltaTime)))
		return -1;

	return 0;
}

_int CBackGround::LateTick(_double fDeltaTime)
{
	if (FAILED(CGameObject::LateTick(fDeltaTime)))
		return -1;
	if (!m_pRenderer)
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return 0;
}

HRESULT CBackGround::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	if (!m_pRcTex)
		return E_FAIL;

	_matrix matWorld, matView, matProj;
	matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	matView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	matProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pRcTex->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pRcTex->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pRcTex->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pRcTex->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pRcTex->Render(0);

	return S_OK;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("BackGround Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBackGround* pInstance = new CBackGround(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("BackGround Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CBackGround::Ready_GameObject(void* pArg)
{
	wstring pTag = (*(wstring*)pArg);

	if (FAILED(m_pTexture->Change_Texture(pTag)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pRcTex);
}

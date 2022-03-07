#include "framework.h"
#include "Player.h"
#include "GameInstance.h"
#include "HierarchyNode.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CPlayer::Tick(_double TimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
		m_pTransform->Go_Straight(TimeDelta);
	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pTransform->Go_Straight(TimeDelta);
	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * -1.f);
	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransform->Rotation_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);

	if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		m_bNomralMapping = true;
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		m_bNomralMapping = false;

	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		m_bTrailOnOff = true;
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		m_bTrailOnOff = false;

	return _int();
}

_int CPlayer::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (m_bTrailOnOff)
		m_pModelCom->Update_CombinedTransformationMatrix(TimeDelta);
	else
		m_pModelCom->Update_CombinedTransformationMatrix(0);

	SetUp_Weapon();

	//if (m_bTrailOnOff)
	//{
	//	_vector pos = m_hadMatrix.r[3];
	//	_vector startpos, endpos;

	//	startpos = XMVector3TransformCoord(XMVectorSet(0.f, -1.f, 0.f, 0.f), m_hadMatrix);
	//	endpos = XMVector3TransformCoord(XMVectorSet(0.f, -0.3f, 0.f, 0.f), m_hadMatrix);

	//	static_cast<CSwordTrail*>(m_pSwordTrail)->AddVertex(startpos, endpos);
	//	m_fTimer = 0.f;
	//}
	//else
	//{
	//	static_cast<CSwordTrail*>(m_pSwordTrail)->Clear_Vertex();
	//}

	return _int();
}

HRESULT CPlayer::Render()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_bool", &m_bNomralMapping, sizeof(_bool));

	//if (FAILED(m_pModelCom->()))
	//	return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom->SetUp_TextureOnShader("g_BiNormalTexture", i, aiTextureType_NORMALS);

		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CPlayer::Render_Shadow()
{
	_matrix world, lightview, lightproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	lightview = XMMatrixTranspose(m_Lightdesc->mLightView);
	lightproj = XMMatrixTranspose(m_Lightdesc->mLightProj);

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_LightProj", &lightproj, sizeof(_matrix));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModelCom->Render(i, 1);
	}

	return S_OK;
}

HRESULT CPlayer::Render_ShadeShadow(ID3D11ShaderResourceView* ShadowMap)
{

	_matrix world, view, proj, lightview, lightproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	lightview = XMMatrixTranspose(m_Lightdesc->mLightView);
	lightproj = XMMatrixTranspose(m_Lightdesc->mLightProj);

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_LightProj", &lightproj, sizeof(_matrix));


	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom->SetUp_TextureOnShader("g_ShadowTexture", ShadowMap);

		m_pModelCom->Render(i, 2);
	}


	return S_OK;
}

HRESULT CPlayer::Render_PBR()
{
	_bool nontex = false;

	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_bool", &m_bNomralMapping, sizeof(_bool));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		switch (i)
		{
		case 0:
			m_pModelCom->SetUp_TextureOnShader("g_PBRTexture", m_vecTextureCom[i]->Get_ShaderResourceView(0));
			break;
		case 1:
			m_pModelCom->SetUp_TextureOnShader("g_PBRTexture", m_vecTextureCom[i]->Get_ShaderResourceView(0));
			break;
		case 2:
			// Cloath
			break;
		case 3:
			// Hair
			break;
		}

		m_pModelCom->Render(i, 3);
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 7.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	m_pTransform->Set_TransformDesc(TransformDesc);

	m_vecTextureCom.resize(2);
	m_vecTextureCom[0] = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");
	m_vecTextureCom[1] = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");

	m_vecTextureCom[0]->Change_Texture(L"PBR_Player_Top");
	m_vecTextureCom[1]->Change_Texture(L"PBR_Player_Down");

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"PlayerModel" , L"Com_Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	m_Lightdesc = g_pGameInstance->Get_LightDesc(0);
	m_pWeponNode = m_pModelCom->Get_BoneMatrix("weapon_r_end");

	_vector vpos = { 3.f,2.f,3.f,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vpos);

	return S_OK;
}

HRESULT CPlayer::SetUp_Weapon()
{
	_matrix		pivot;

	pivot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(180.f), XMConvertToRadians(180.f), XMConvertToRadians(180.f)) * m_pModelCom->Get_PivotMatrix() * XMMatrixRotationY(XMConvertToRadians(-180.f));

	m_hadeMatrix = m_pWeponNode->Get_CombinedMatrix() * pivot * m_pTransform->Get_WorldMatrix();

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& iter : m_vecTextureCom)
		Safe_Release(iter);
	m_vecTextureCom.clear();

	Safe_Release(m_pModelCom);
}

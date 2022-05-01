#include "pch.h"
#include "Loot_Shield.h"
#include "ItemData.h"
#include "Client_Function.h"
#include "InventoryData.h"
#include "Material.h"

CLoot_Shield::CLoot_Shield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CInteractableObject(_pDevice, _pDeviceContext)
{
}

CLoot_Shield::CLoot_Shield(const CLoot_Shield& _rhs)
	:CInteractableObject(_rhs)
{
	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");
}

HRESULT CLoot_Shield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_LootShield", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_LootShield", pMtrl);


	return S_OK;
}

HRESULT CLoot_Shield::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bTakable = false;
	m_bInteractDead = false;

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{-155.f, 22.4f, 218.1f, 1.f});
	m_pTransform->Scaling(_fvector{ 1.5f, 1.4f, 1.5f, 1.f });

	m_pTransform->Rotation_Axis(CTransform::STATE_RIGHT, 55.f);

	setActive(true);

	return S_OK;
}

_int CLoot_Shield::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	if (false == IsTakable())
	{
		SetTakableState(true);
		_vector Pos = { -0.04f, -0.1f, 0.f, 0.f };
		Active_Effect((_uint)EFFECT::BOX, Pos);
	}
	else if (true == IsTakable())
	{
		m_bLateTick = true;
	}


	return _int();
}

_int CLoot_Shield::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ -155.f, 23.0f, 218.1f, 1.f });

	m_pTransform->Rotation_Axis(CTransform::STATE_LOOK, (_float)_dDeltaTime * 1.5f);

	return _int();
}

HRESULT CLoot_Shield::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

	SCB desc;
	ZeroMemory(&desc, sizeof(SCB));

	desc.metalic = 0.6f;
	desc.roughness = -0.1f;
	desc.color = _float4(0.156f, 0.76f, 1.f, 1.f);
	desc.empower = 20.f;


	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_Metalic", &desc.metalic, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_Roughness", &desc.roughness, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_AO", &desc.ao, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_color", &desc.color, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &desc.empower, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 0)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoot_Shield::Ready_Components()
{
	/* for. Transform Com */
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 1.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_FlyingShield", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationZ(XMConvertToRadians(45.0f));
	m_pModel->Set_PivotMatrix(matPivot);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_LootShield"), 0);

	return S_OK;
}

_bool CLoot_Shield::IsInteractable(void)
{
	if (false == IsTakable())
		return false;

	m_pPlayer = *g_pGameInstance->getObjectList(g_pGameInstance->getCurrentLevel(), L"Layer_Silvermane")->begin();
	assert(m_pPlayer);

	_float dist = MathUtils::Length(m_pPlayer, this);

	if (dist <= m_interactDist)
		return true;

	return false;
}

_bool CLoot_Shield::HasInteractionUI(void)
{
	return false;
}

_float CLoot_Shield::GetRatioForUI(void)
{
	return _float();
}

_vector CLoot_Shield::GetGuideUIPos(void)
{
	return _vector();
}

void CLoot_Shield::BeginInteraction(void)
{
	__super::BeginInteraction();
	Take();
}

void CLoot_Shield::UpdateInteraction(_double _dTimeDelta)
{
	__super::UpdateInteraction(_dTimeDelta);
}

void CLoot_Shield::EndInteraction(void)
{
	__super::EndInteraction();
}

void CLoot_Shield::Take(void)
{
	_vector pivot = { 0.f, -0.05f, 0.f, 0.f };
	Active_Effect((_uint)EFFECT::EAT_ITEM, pivot);

	m_bInteractDead = true;
	m_bTakable = false;
	setActive(false);
	
	STOP_SOUND(CHANNEL::Equip_Shield);
	PLAY_SOUND(L"Item_PickUp", CHANNEL::Equip_Shield);
	static_cast<CSilvermane*>(m_pPlayer)->Loot_Shield();
}

void CLoot_Shield::SetTakableState(_bool bState)
{
	m_bTakable = bState;
}

_bool CLoot_Shield::IsTakable(void)
{
	return m_bTakable;
}

CLoot_Shield* CLoot_Shield::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CLoot_Shield* pInstance = new CLoot_Shield(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CLoot_Shield Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLoot_Shield::Clone(const _uint _iSceneID, void* _pArg)
{
	CLoot_Shield* pInstance = new CLoot_Shield(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CLoot_Shield Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoot_Shield::Free()
{
	__super::Free();

	m_pInventoryData = nullptr;
}
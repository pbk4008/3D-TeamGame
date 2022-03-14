#include "framework.h"
#include "pch.h"
#include "UI_Monster_Panel.h"
#include "GameInstance.h"

#include "UI_Monster_Back.h"
#include "UI_Monster_Level.h"
#include "UI_Monster_HpBar.h"

CUI_Monster_Panel::CUI_Monster_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CUI_Monster_Panel::CUI_Monster_Panel(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Monster_Panel::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_Panel::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(PANELDESC));
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	//MonsterBar Back
	CUI_Monster_Back::UIACTIVEDESC Desc;
	_tcscpy_s(Desc.UIDesc.TextureTag, L"Texture_Monster_Back");
	Desc.UIDesc.IDTag = 14;
	Desc.UIDesc.bMinus = true;
	Desc.UIDesc.fAngle = 0.36f;
	Desc.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc.UIDesc.fSize = { 1.f, 1.f };
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Back", &Desc, 
		(CGameObject**)&m_pUIBack)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_Level::UIACTIVEDESC Desc2;
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Monster_Level");
	Desc2.UIDesc.IDTag = 13;
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc2.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_Level", &Desc2, 
		(CGameObject**)&m_pUILevel)))
		return E_FAIL;

	//MonsterBar Level
	CUI_Monster_HpBar::UIACTIVEDESC Desc3;
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Monster_HpBar");
	Desc3.UIDesc.IDTag = 14;
	Desc3.UIDesc.bMinus = true;
	Desc3.UIDesc.fAngle = 0.36f;
	Desc3.UIDesc.fPos = { 0.f, 0.f, 0.f };
	Desc3.UIDesc.fSize = { 1.f, 1.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Monster_HpBar", &Desc3,
		(CGameObject**)&m_pUIHpBar)))
		return E_FAIL;

	return S_OK;
}

_int CUI_Monster_Panel::Tick(_double TimeDelta)
{
	if (FAILED(__super::Tick(TimeDelta)))
		return -1;

	//Panel pos Setting
	_matrix SettingMat = XMMatrixIdentity();
	SettingMat.r[3] = { 0.f, 2.f, 0.f , 1.f };
	_matrix TargetSettinMat = SettingMat * XMLoadFloat4x4(&m_TargetMatrix); //몬스터(타겟)위치
	m_pTransform->Set_WorldMatrix(TargetSettinMat);

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	//Panel Size
	_vector vScale = { 1.f,1.f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	//UI Back
	CTransform* BackTransform = (CTransform*)m_pUIBack->Get_Component(L"Com_Transform");
	_matrix Backmat = XMMatrixIdentity();
	Backmat.r[0] = XMVectorSetX(Backmat.r[0], 3.f);
	Backmat.r[1] = XMVectorSetY(Backmat.r[1], 0.40f);
	Backmat.r[3] = { 0.f, 0.f, 0.f, 1.f };
	BackTransform->Set_WorldMatrix(Backmat * m_pTransform->Get_WorldMatrix());
	
	//UI Level
	CTransform* LevelTransform = (CTransform*)m_pUILevel->Get_Component(L"Com_Transform");
	_matrix Levelmat = XMMatrixIdentity();
	Levelmat.r[0] = XMVectorSetX(Levelmat.r[0], 0.6f);
	Levelmat.r[1] = XMVectorSetY(Levelmat.r[1], 0.6f);
	Levelmat.r[3] = { -1.55f, 0.11f, 0.f, 1.f };
	LevelTransform->Set_WorldMatrix(Levelmat * m_pTransform->Get_WorldMatrix());

	//UI HpBar
	CTransform* HpBarTransform = (CTransform*)m_pUIHpBar->Get_Component(L"Com_Transform");
	_matrix HpBarmat = XMMatrixIdentity();
	HpBarmat.r[0] = XMVectorSetX(HpBarmat.r[0], 2.5f);
	HpBarmat.r[1] = XMVectorSetY(HpBarmat.r[1], 0.40f);
	HpBarmat.r[3] = { 0.f, 0.0f, -0.001f, 1.f };
	HpBarTransform->Set_WorldMatrix(HpBarmat * m_pTransform->Get_WorldMatrix());

	return 0;
}

_int CUI_Monster_Panel::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}
	return _int();
}

HRESULT CUI_Monster_Panel::Render()
{
	return S_OK;
}

HRESULT CUI_Monster_Panel::SetUp_Components()
{
	m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Monster_Panel* CUI_Monster_Panel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_Panel* pInstance = new CUI_Monster_Panel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_Panel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Panel::Clone(void* pArg)
{
	CUI_Monster_Panel* pInstance = new CUI_Monster_Panel(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_Panel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Panel::Free()
{
	__super::Free();
}

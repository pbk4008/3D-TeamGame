#include "framework.h"
#include "pch.h"
#include "UI_Shield_Meter.h"
#include "GameInstance.h"

CUI_Shield_Meter::CUI_Shield_Meter(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI_Monster_Panel(pDevice,pDeviceContext)
{
}

CUI_Shield_Meter::CUI_Shield_Meter(const CUI_Monster_Panel& rhs)
	: CUI_Monster_Panel(rhs)
{
}

HRESULT CUI_Shield_Meter::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Shield_Meter::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* �������� �����ͷ� ���� ���ϴ� �� ���� */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vPos = { m_Desc.UIDesc.fPos.x - (g_iWinCx >> 1),-m_Desc.UIDesc.fPos.y + (g_iWinCy >> 1), m_Desc.UIDesc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);


	m_iTextureNum = 15; //�� ���ִ� ������


	return S_OK;
}

_int CUI_Shield_Meter::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	if (g_pGameInstance->getkeyDown(DIK_NUMPAD4))
	{
		m_bUseShield = true; //�ǵ带��������� , ��ü �� ������������ �ǵ带 ����. 
		m_bCoolTime = true;
	}

	if (m_bUseShield)
	{
		m_iTextureNum = 0.f; //�ǵ带 �����ϱ� �� ���ְ�
		m_bUseShield = false;
	}

	if (m_bCoolTime) //�ǵ带��������� ��Ÿ���������� 
	{
		m_fFillTimeAcc += TimeDelta;
	}

	if (0.5f <= m_fFillTimeAcc) //0.5�ʰ� ������ ��ĭ��ä����
	{

		if (15 > m_iTextureNum) 
		{
			++m_iTextureNum;
		}
		else if (15 <= m_iTextureNum)
		{
			//���������� �� á���� ��Ÿ�ӳ�
			m_iTextureNum = 15;
			m_bCoolTime = false;
		}

		m_fFillTimeAcc = 0.f;
	}

	return 0;
}

_int CUI_Shield_Meter::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		if (g_pInvenUIManager->Get_OpenCheck() == false)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);
	}
	return _int();
}

HRESULT CUI_Shield_Meter::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_iTextureNum);

	m_pBuffer->Render(1);
	
	return S_OK;
}

HRESULT CUI_Shield_Meter::SetUp_Components()
{
	m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
		return E_FAIL;



	return S_OK;
}

CUI_Shield_Meter* CUI_Shield_Meter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* ������ü �����Ҷ� �ʱ�ȭ */
	CUI_Shield_Meter* pInstance = new CUI_Shield_Meter(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Shield_Meter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Shield_Meter::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Shield_Meter* pInstance = new CUI_Shield_Meter(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Shield_Meter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shield_Meter::Free()
{
	__super::Free();
}

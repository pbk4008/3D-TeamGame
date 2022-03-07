#include "pch.h"
#include "ToolUI.h"

CToolUI::CToolUI()
	: m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_bClicked(false)
	, m_eTag(UITAG::UI_END)
{
}

CToolUI::CToolUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_bClicked(false)
	, m_eTag(UITAG::UI_END)
{
}

CToolUI::CToolUI(const CToolUI& rhs)
	: CGameObject(rhs)
	, m_pBuffer(rhs.m_pBuffer)
	, m_pTexture(nullptr)
	, m_bClicked(false)
	, m_eTag(rhs.m_eTag)
{
	Safe_AddRef(m_pBuffer);
}

HRESULT CToolUI::Init_GameObject(COMINDEX* pIndex)
{
	if (FAILED(CGameObject::Init_GameObject(pIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolUI::Init_Clone(void* pArg)
{
	void* pTmpArg = nullptr;
	if (FAILED(CGameObject::Init_Clone(pTmpArg)))
		return E_FAIL;

	if (pArg)
	{
		memcpy(m_pComponentIndex, (COMINDEX*)pArg, sizeof(COMINDEX) * (_uint)COMPONENTID::COMPONENT_END);
		if(FAILED(Add_Component<CVIBuffer_Rect>((_uint)TOOLSCENEID::TOOL_DEFORM,COMPONENTID::VIBUFFER_RECT,m_pComponentIndex[(_uint)COMPONENTID::VIBUFFER_RECT].iStartIndex,&m_pBuffer)))
			return E_FAIL;
		if (FAILED(Add_Component<CTexture>((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::TEXTURE, m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex, &m_pTexture)))
			return E_FAIL;
	}
	_vector vScalce = m_pTexture->getLen(0);
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_RIGHT, XMVectorSet(XMVectorGetX(vScalce) * 2.f, 0.f, 0.f, 0.f));
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_UP, XMVectorSet(0.f, XMVectorGetY(vScalce) * 2.f, 0.f, 0.f));

	return S_OK;
}

_int CToolUI::Update_GameObject(_float fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);
	return _int();
}

HRESULT CToolUI::Render_GameObject()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	_matrix matWorld = XMMatrixTranspose(m_pTransform->getWorldMatrix());
	_matrix matView = XMMatrixTranspose(pInstance->GetTransform(L"SubCamera", TRANSMATRIXSTATE::D3DT_VIEW));
	_matrix matProj = XMMatrixTranspose(pInstance->GetTransform(L"SubCamera", TRANSMATRIXSTATE::D3DT_PROJ));
	_matrix matIdentity = XMMatrixIdentity();

	_float4 vColor;
	if (m_bClicked)
	{
		vColor = { 0.f, 1.f, 0.f, 1.f };
		m_pBuffer->SetUp_ValueOnShader("g_vClickedColor", &vColor, sizeof(_float4));
	}
	else
	{
		vColor = { 1.f,1.f,1.f,1.f };
		m_pBuffer->SetUp_ValueOnShader("g_vClickedColor", &vColor, sizeof(_float4));
	}
	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &matIdentity, sizeof(_matrix));
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));
	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(0);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_bool CToolUI::Click_UI(_fvector vMousePos)
{
	_vector vPos = m_pTransform->getState(CTransform::TRANS_STATE::STATE_POS);
	_vector vLen = m_pTexture->getLen(0);

	_vector vRightDown = vPos + vLen;
	_vector vLeftUp = vPos - vLen;

	if(XMVectorGetX(vLeftUp) <XMVectorGetX(vMousePos)
		&&XMVectorGetY(vLeftUp) < XMVectorGetY(vMousePos)
		&&XMVectorGetX(vRightDown) > XMVectorGetX(vMousePos)
		&&XMVectorGetY(vRightDown) > XMVectorGetY(vMousePos))
		return true;

	return false;
}

void CToolUI::Change_Tranform(_fvector vScale, _fvector vPosition)
{
	m_pTransform->Set_Scale(vScale);
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, vPosition);
}
void CToolUI::Change_ToolValue(_float2* vScale, _float2* vPos, _uint* iTagNum)
{
	_vector vTransformScale = m_pTransform->getScale();
	_vector vTransformPos = m_pTransform->getState(CTransform::TRANS_STATE::STATE_POS);

	XMStoreFloat2(vScale, vTransformScale);
	XMStoreFloat2(vPos, vTransformPos);

	*iTagNum = (_uint)m_eTag;
}

CGameObject* CToolUI::Clone(void* pArg)
{
	CToolUI* pInstance = new CToolUI(*this);
	if (FAILED(pInstance->Init_Clone(pArg)))
	{
		MSG_BOX(L"CToolUI Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CToolUI* CToolUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex)
{
	CToolUI* pInstance = new CToolUI(pDevice,pDeviceContext);
	if (FAILED(pInstance->Init_GameObject(pIndex)))
	{
		MSG_BOX(L"CToolUI Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolUI::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}

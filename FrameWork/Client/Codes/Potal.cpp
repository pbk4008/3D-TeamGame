#include "pch.h"
#include "Meteor.h"
#include "..\Headers\Potal.h"

CPotal::CPotal()
	: m_pRect(nullptr)
	, m_pDiffuseTexture(nullptr)
	, m_pMaskTexture(nullptr)
	, m_bCreate(false)
	, m_fAccRetain(0.f)
{
}

CPotal::CPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pRect(nullptr)
	, m_pDiffuseTexture(nullptr)
	, m_pMaskTexture(nullptr)
	, m_bCreate(false)
	, m_fAccRetain(0.f)
{
}

CPotal::CPotal(const CPotal& rhs)
	: CGameObject(rhs)
	, m_pRect(rhs.m_pRect)
	, m_pDiffuseTexture(rhs.m_pDiffuseTexture)
	, m_pMaskTexture(rhs.m_pMaskTexture)
	, m_bCreate(rhs.m_bCreate)
	, m_fAccRetain(0.f)
{
	Safe_AddRef(m_pRect);
	Safe_AddRef(m_pDiffuseTexture);
	Safe_AddRef(m_pMaskTexture);
}

HRESULT CPotal::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
	{
		MSGBOX("NativeConstruct_Prototype Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPotal::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("NativeConstruct Fail");
		return E_FAIL;
	}

	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVectorZero());
	m_pTransform->Set_State(CTransform::STATE_UP, XMVectorZero());
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVectorZero());

	//m_pTransform->Scaling(XMVectorSet(100.f, 100.f, 100.f, 0.f));
	_vector vPos = (*(_vector*)_pArg);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

_int CPotal::Tick(_double _dDeltaTime)
{
	Scaling(_dDeltaTime);
	Create_Meteor(_dDeltaTime);
	Remove_Portal(_dDeltaTime);

	return _int();
}

_int CPotal::LateTick(_double _dDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);


	return _int();
}

HRESULT CPotal::Render()
{
	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pRect->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;
	
	if (FAILED(m_pRect->SetUp_TextureOnShader("g_DiffuseTexture", m_pDiffuseTexture->Get_ShaderResourceView())))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_TextureOnShader("g_MaskTexture", m_pMaskTexture->Get_ShaderResourceView())))
		return E_FAIL;

	m_pRect->Render(0);

	return S_OK;
}

HRESULT CPotal::Ready_Component()
{
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Buffer_Potal", L"Rect_Buffer", (CComponent**)&m_pRect)))
		return E_FAIL;

	wstring wstrTextureTag = L"Potal_Diffuse";
	g_pGameInstance->Add_Texture(m_pDevice, wstrTextureTag, L"../bin/Resources/Texture/Portal/T_Portal_Space_D.dds");
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Texture", L"DiffuseTexture", (CComponent**)&m_pDiffuseTexture)))
		return E_FAIL;
	m_pDiffuseTexture->Change_Texture(wstrTextureTag);

	g_pGameInstance->Add_Texture(m_pDevice, wstrTextureTag, L"../bin/Resources/Texture/Portal/T_Portal_SpaceMask_M.dds");
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Texture", L"MaskTexture", (CComponent**)&m_pMaskTexture)))
		return E_FAIL;
	m_pMaskTexture->Change_Texture(wstrTextureTag);

	return S_OK;
}

_uint CPotal::Scaling(_double dDeltaTime)
{
	if (!m_pTransform)
		return -1;

	_float fSize = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);

	if (!m_bCreate)
	{
		if(fSize<3.f)
			fSize += dDeltaTime*10.f;
		else
			fSize += dDeltaTime*4.5f;
	}
	else
		fSize -= (_float)dDeltaTime *5.f;


	_matrix matScale = XMMatrixScaling(fSize, fSize, fSize);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, matScale.r[0]);
	m_pTransform->Set_State(CTransform::STATE_UP, matScale.r[1]);
	m_pTransform->Set_State(CTransform::STATE_LOOK, matScale.r[2]);

	return _uint();
}

_uint CPotal::Create_Meteor(_double dDeltaTime)
{
	if (!m_bCreate)
	{
		_float fSize = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
		if (fSize >= 6.f)
		{
			_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Meteor", L"Proto_GameObject_Weapon_Meteor",&vPos)))
				MSGBOX("Create_Meteor Fail");
			m_bCreate = true;
		}
	}
	else
	{
		m_fAccRetain += dDeltaTime;
		if (m_fAccRetain > 1.f)
		{
			m_fAccRetain = 0.f;
			m_bRetain = true;
		}
	}
	return _uint();
}

_uint CPotal::Remove_Portal(_double dDeltaTime)
{
	if (m_bCreate&&m_bRetain)
	{
		Scaling(dDeltaTime);
		_float fSize = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
		if (fSize < 0.f)
		{
			m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVectorZero());
			m_pTransform->Set_State(CTransform::STATE_UP, XMVectorZero());
			m_pTransform->Set_State(CTransform::STATE_LOOK, XMVectorZero());
			m_bRemove = true;
		}
	}
	return _uint();
}

CPotal* CPotal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContex)
{
	CPotal* pInsance = new CPotal(pDevice, pDeviceContex);
	if (FAILED(pInsance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPotal Create Faile");
		Safe_Release(pInsance);
	}
	return pInsance;
}

CGameObject* CPotal::Clone(_uint iSeneid, void* pArg)
{
	CPotal* pInsance = new CPotal(*this);
	if (FAILED(pInsance->NativeConstruct(iSeneid, pArg)))
	{
		MSGBOX("CPotal Clone Faile");
		Safe_Release(pInsance);
	}
	return pInsance;
}

void CPotal::Free()
{
	Safe_Release(m_pRect);
	Safe_Release(m_pDiffuseTexture);
	Safe_Release(m_pMaskTexture);

	CGameObject::Free();
}

#include "pch.h"
#include "..\Headers\Potal.h"
#include "Monster_Bastion_Sword.h"
#include "Monster_Bastion_Shooter.h"
#include "Monster_Bastion_2HSword.h"

CPotal::CPotal()
	: m_pRect(nullptr)
	, m_pDiffuseTexture(nullptr)
	, m_pMaskTexture(nullptr)
	, m_bCreate(false)
	, m_fAccRetain(0.f)
	, m_bOpenCheck(false)
	, m_iMonTag(0)
	, m_bRetain(false)
{
}

CPotal::CPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pRect(nullptr)
	, m_pDiffuseTexture(nullptr)
	, m_pMaskTexture(nullptr)
	, m_bCreate(false)
	, m_fAccRetain(0.f)
	, m_bOpenCheck(false)
	, m_iMonTag(0)
	, m_bRetain(false)
{
}

CPotal::CPotal(const CPotal& rhs)
	: CGameObject(rhs)
	, m_pRect(rhs.m_pRect)
	, m_pDiffuseTexture(rhs.m_pDiffuseTexture)
	, m_pMaskTexture(rhs.m_pMaskTexture)
	, m_bCreate(rhs.m_bCreate)
	, m_fAccRetain(0.f)
	, m_bOpenCheck(false)
	, m_iMonTag(0)
	, m_bRetain(false)
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

	setActive(false);
	return S_OK;
}

_int CPotal::Tick(_double _dDeltaTime)
{
	if (!m_bOpenCheck)
		return 0;

	Scaling(_dDeltaTime);
	Spawn_Monster(_dDeltaTime);
	Remove_Portal(_dDeltaTime);

	return _int();
}

_int CPotal::LateTick(_double _dDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHANB, this);

	return _int();
}

HRESULT CPotal::Render()
{
	_matrix smatWorld, smatView, smatProj;

	_matrix ViewMatrix;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	ViewMatrix = g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	
	_float vScaleX = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
	_float vScaleZ = m_pTransform->Get_Scale(CTransform::STATE_LOOK);
	_vector vRight = XMVector3Normalize(ViewMatrix.r[0]);
	_vector vLook = XMVector3Normalize(ViewMatrix.r[2]);
	vRight *= vScaleX;
	vLook *= vScaleZ;
	
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, vLook);

	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pRect->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
		return E_FAIL;

	//if (FAILED(m_pRect->SetUp_ValueOnShader("g_bOpenCheck", &m_bOpenCheck, sizeof(_bool))))
	//	return E_FAIL;
	//if (FAILED(m_pRect->SetUp_ValueOnShader("g_fAccTime", &m_fAccTime, sizeof(_float))))
	//	return E_FAIL;
	
	if (FAILED(m_pRect->SetUp_TextureOnShader("g_DiffuseTexture", m_pDiffuseTexture->Get_ShaderResourceView())))
		return E_FAIL;
	if (FAILED(m_pRect->SetUp_TextureOnShader("g_MaskTexture", m_pMaskTexture->Get_ShaderResourceView())))
		return E_FAIL;

	m_pRect->Render(0);

	return S_OK;
}

void CPotal::Open_Potal(_uint iMonTag, _fvector vPos)
{
	m_bOpenCheck = true;
	m_iMonTag = iMonTag;

	_vector vTmp = vPos;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vTmp);
	
	_vector vecZero = XMVectorZero();

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vecZero);
	m_pTransform->Set_State(CTransform::STATE_UP, vecZero);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vecZero);

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

	wstrTextureTag = L"Potal_Mask";
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

	/*if (!m_bCreate && !m_bOpenCheck)
		m_fAccTime += dDeltaTime;*/


	_float fSize = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);

	if (!m_bCreate)
	{
		if(fSize<3.f)
			fSize += (_float)dDeltaTime*10.f;
		else
			fSize += (_float)dDeltaTime*4.5f;
	}
	else
		fSize -= (_float)dDeltaTime *5.f;


	_matrix matScale = XMMatrixScaling(fSize, fSize, fSize);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, matScale.r[0]);
	m_pTransform->Set_State(CTransform::STATE_UP, matScale.r[1]);
	m_pTransform->Set_State(CTransform::STATE_LOOK, matScale.r[2]);

	return _uint();
}

_uint CPotal::Spawn_Monster(_double dDeltaTime)
{
	if (!m_bCreate)
	{
		_float fSize = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
		if (fSize >= 6.f)
		{
			m_bCreate = true;
			_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			list<CGameObject*>* pLayer = g_pGameInstance->getObjectList(m_iSceneID, L"Layer_PortalMonster");

			if (!pLayer)
				return -1;

			for (auto& pMon : *pLayer)
			{
				if (!pMon->getActive() && pMon->getTag() == m_iMonTag)
				{
					if (m_iMonTag == (_uint)GAMEOBJECT::MONSTER_1H)
					{
						pMon->setActive(true);
						static_cast<CMonster_Bastion_Sword*>(pMon)->Set_SpawnPosition(vPos);
					}
					else if (m_iMonTag == (_uint)GAMEOBJECT::MONSTER_SHOOTER)
					{
						pMon->setActive(true);
						static_cast<CMonster_Bastion_Shooter*>(pMon)->Set_SpawnPosition(vPos);
					}
					else if (m_iMonTag == (_uint)GAMEOBJECT::MONSTER_2H)
					{
						pMon->setActive(true);
						static_cast<CMonster_Bastion_2HSword*>(pMon)->Set_SpawnPosition(vPos);
					}
					else if (m_iMonTag == (_uint)GAMEOBJECT::MONSTER_HEALER)
					{
						pMon->setActive(true);
						static_cast<CMonster_Bastion_2HSword*>(pMon)->Set_SpawnPosition(vPos);
					}
					else if (m_iMonTag == (_uint)GAMEOBJECT::MONSTER_SPEAR)
					{
						pMon->setActive(true);
						static_cast<CMonster_Bastion_2HSword*>(pMon)->Set_SpawnPosition(vPos);
					}
					break;
				}
			}
		}	
	}
	else
	{
		m_fAccRetain += (_float)dDeltaTime;
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
		if (fSize < 0.1f)
		{
			m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVectorZero());
			m_pTransform->Set_State(CTransform::STATE_UP, XMVectorZero());
			m_pTransform->Set_State(CTransform::STATE_LOOK, XMVectorZero());
			m_bOpenCheck = false;
			m_bCreate = false;
			m_bRetain = false;
			//m_bRemove = true;
			setActive(false);
		}
		/*	m_bOpenCheck = true;
			m_fAccTime += dDeltaTime;
			if (m_fAccTime > 1.f)
				m_bRemove = true;*/
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

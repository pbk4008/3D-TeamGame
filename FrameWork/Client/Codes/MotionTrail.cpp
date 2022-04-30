#include "pch.h"
#include "MotionTrail.h"
#include "Silvermane.h"

CMotionTrail::CMotionTrail(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CMotionTrail::CMotionTrail(const CMotionTrail& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMotionTrail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMotionTrail::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_pGradientTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_pGradientTex->Change_Texture(L"DissovleGradient"))) MSGBOX("Failed to Change Texture DissovleTex");
	 
	ZeroMemory(m_bonematrix, sizeof(_matrix) * 256);

	return S_OK;
}

_int CMotionTrail::Tick(_double _dDeltaTime)
{
	if (m_runcheck == true)
		m_lifetime -= (_float)_dDeltaTime * 2.f;
	else
		m_lifetime -= (_float)_dDeltaTime * 2.5f;

	if (m_lifetime <= 0.f)
	{
		m_lifetime = 1.f;
		m_bActive = false;
	}

	return _int();
}

_int CMotionTrail::LateTick(_double _dDeltaTime)
{
	if (m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_MOTIONTRAIL, this);
	}

	return _int();
}

HRESULT CMotionTrail::Render_MotionTrail()
{
	wstring camtag = g_pGameInstance->Get_BaseCameraTag();
	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));
	rimdesc.rimcheck = m_rimcheck;
	rimdesc.rimcol = _float3(0.7529f, 0.7529f, 0.7529f);
	rimdesc.rimintensity = m_rimintensity;
	XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(g_pGameInstance->Get_CamPosition(camtag) - m_worldamt.r[3]));


	if (m_runcheck == true || m_throwchck == true)
	{
		if (XMVector3Equal(m_shieldworldmat.r[3], XMMatrixIdentity().r[3]) != true)
		{
			XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(g_pGameInstance->Get_CamPosition(camtag) - m_shieldworldmat.r[3]));
			Set_ContantBuffer(m_pShield, m_shieldworldmat, rimdesc);
			for (_uint i = 0; i < m_pShield->Get_NumMeshContainer(); ++i)
			{
				if (FAILED(m_pShield->Render(i, 4))) MSGBOX("Fialed To Rendering Shield MotionTrail");
			}
		}
	}
	else
	{
		Set_ContantBuffer(m_pModel, m_worldamt, rimdesc);
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_TrailBoneMatrices", &m_bonematrix, sizeof(_matrix) * 256))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");

		for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pModel->Render(i, 4))) MSGBOX("Fialed To Rendering Player MotionTrail");
		}

		if (XMVector3Equal(m_weaponworldmat.r[3], XMMatrixIdentity().r[3]) != true)
		{
			_matrix weaponworld;
			weaponworld = XMMatrixTranspose(m_weaponworldmat);
			XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(g_pGameInstance->Get_CamPosition(camtag) - m_weaponworldmat.r[3]));
			Set_ContantBuffer(m_pWeapon, m_weaponworldmat, rimdesc);
			for (_uint i = 0; i < m_pWeapon->Get_NumMeshContainer(); ++i)
			{
				if (FAILED(m_pWeapon->Render(i, 4))) MSGBOX("Fialed To Rendering Weapon MotionTrail");
			}
		}
	}

	return S_OK;
}

void CMotionTrail::Set_BoneMat(_fmatrix* bone)
{
	memcpy(m_bonematrix, bone, sizeof(_matrix) * 256);
}

void CMotionTrail::Set_Info(_fmatrix world, _fmatrix weapon, _fmatrix shield, _float UVdvid, CModel* pCurWeapon)
{
	m_worldamt = world;
	m_weaponworldmat = weapon;
	m_shieldworldmat = shield;
	m_UVdvid = UVdvid;
	m_pWeapon = pCurWeapon;
}

void CMotionTrail::Set_Model(CModel* pModel, CModel* pWeapon,CModel* pShield)
{
	m_pModel = pModel;
	m_pWeapon = pWeapon;
	m_pShield = pShield;
}

HRESULT CMotionTrail::Set_ContantBuffer(CModel* pmodel, _fmatrix worldmat, RIM& rimdesc)
{
	_matrix smatWorld, smatView, smatProj;

	smatWorld = XMMatrixTranspose(worldmat);

	wstring camtag = g_pGameInstance->Get_BaseCameraTag();

	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(camtag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(camtag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(pmodel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	if (FAILED(pmodel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	if (FAILED(pmodel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");

	if (FAILED(pmodel->SetUp_ValueOnShader("g_rimlightcheck", &rimdesc.rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	if (FAILED(pmodel->SetUp_ValueOnShader("g_rimintensity", &rimdesc.rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	if (FAILED(pmodel->SetUp_ValueOnShader("g_rimcolor", &rimdesc.rimcol, sizeof(_float3)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	if (FAILED(pmodel->SetUp_ValueOnShader("g_camdir", &rimdesc.camdir, sizeof(_float4)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");
	
	/*if (FAILED(pmodel->SetUp_ValueOnShader("g_Fade", &m_lifetime, sizeof(_float)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");*/
	if (FAILED(pmodel->SetUp_ValueOnShader("g_UVdvid", &m_UVdvid, sizeof(_float)))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");

	if (FAILED(pmodel->SetUp_TextureOnShader("g_GradientTex", m_pGradientTex))) MSGBOX("Failed To Apply MotionTrail ConstantBuffer");

	return S_OK;
}

CMotionTrail* CMotionTrail::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMotionTrail* pInstance = new CMotionTrail(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMotionTrail Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMotionTrail::Clone(const _uint _iSceneID, void* _pArg)
{
	CMotionTrail* pInstance = new CMotionTrail(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CMotionTrail Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMotionTrail::Free()
{
	__super::Free();
	//Safe_Release(m_pWeapon);
	//Safe_Release(m_pShield);
	Safe_Release(m_pGradientTex);
}

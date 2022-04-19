#include "pch.h"
#include "MotionTrail.h"

CMotionTrail::CMotionTrail(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMotionTrail::CMotionTrail(const CMotionTrail& _rhs)
	: CActor(_rhs)
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

	if (FAILED(Ready_Component()))
		return E_FAIL;

	ZeroMemory(m_bonematrix, sizeof(_matrix) * 256);

	return S_OK;
}

_int CMotionTrail::Tick(_double _dDeltaTime)
{
	m_lifetime += (_float)g_fDeltaTime;
	if (m_lifetime >= 2.0)
	{
		m_lifetime = 0.f;
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

HRESULT CMotionTrail::Render()
{
	_matrix smatWorld, smatView, smatProj;

	smatWorld = XMMatrixTranspose(m_worldamt);
	smatView = XMMatrixTranspose(m_viewmat);
	smatProj = XMMatrixTranspose(m_projmat);

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_TrailBoneMatrices", &m_bonematrix, sizeof(_matrix) * 256))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));
	rimdesc.rimcheck = m_rimcheck;
	rimdesc.rimcol = _float3(0.f, 1.0f, 0);
	/*CActor::SetRimIntensity(g_fDeltaTime * -1.f);*/
	rimdesc.rimintensity = m_rimintensity; // intensity ³·À» ¼ö·Ï °úÇÏ°Ô ºû³²
	XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(m_position - m_camposition));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimlightcheck", &rimdesc.rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimintensity", &rimdesc.rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimcolor", &rimdesc.rimcol, sizeof(_float3)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_camdir", &rimdesc.camdir, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 4))) MSGBOX("Fialed To Rendering Silvermane");
	}

	return S_OK;
}

HRESULT CMotionTrail::Render_MotionTrail()
{
	_matrix smatWorld,smatView, smatProj;

	smatWorld = XMMatrixTranspose(m_worldamt);

	wstring camtag = g_pGameInstance->Get_BaseCameraTag();

	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(camtag,TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(camtag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_TrailBoneMatrices", &m_bonematrix, sizeof(_matrix) * 256))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(RIM));
	rimdesc.rimcheck = m_rimcheck;
	rimdesc.rimcol = _float3(0.f, 1.0f, 0);
	rimdesc.rimintensity = m_rimintensity; // intensity ³·À» ¼ö·Ï °úÇÏ°Ô ºû³²
	XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(m_position - m_camposition));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimlightcheck", &rimdesc.rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimintensity", &rimdesc.rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimcolor", &rimdesc.rimcol, sizeof(_float3)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_camdir", &rimdesc.camdir, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 4))) MSGBOX("Fialed To Rendering Silvermane");
	}

	return S_OK;
}

void CMotionTrail::Set_BoneMat(_fmatrix* bone)
{
	memcpy(m_bonematrix, bone, sizeof(_matrix) * 256);
}

void CMotionTrail::Set_Info(_fmatrix world, _vector position, _vector campostion)
{
	m_worldamt = world;
	m_position = position;
	m_camposition = campostion;
}

void CMotionTrail::Set_Model(CModel* pModel)
{
	m_pModel = pModel;
	Safe_AddRef(m_pModel);
}

HRESULT CMotionTrail::Ready_Component()
{
	//if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Silvermane_Bin", L"Model", (CComponent**)&m_pModel)))
	//	return E_FAIL;

	//_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//m_pModel->Set_PivotMatrix(matPivot);

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
	CActor::Free();
}

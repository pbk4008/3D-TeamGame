#include "pch.h"
#include "MeshEffect.h"

#include "Material.h"

CMeshEffect::CMeshEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CMeshEffect::CMeshEffect(const CMeshEffect& _rhs)
	: CGameObject(_rhs)
	, m_pMaterial(_rhs.m_pMaterial)
	, m_pTexture(_rhs.m_pTexture)
	, m_pNormalTex(_rhs.m_pNormalTex)
	, m_pNoiseTex(_rhs.m_pNoiseTex)
	, m_pMaskTex(_rhs.m_pMaskTex)
{
	Safe_AddRef(m_pMaterial);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pNormalTex);
	Safe_AddRef(m_pNoiseTex);
	Safe_AddRef(m_pMaskTex);
}

HRESULT CMeshEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pTexture)
		return E_FAIL;
	m_pNormalTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pNormalTex)
		return E_FAIL;
	m_pNoiseTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pNoiseTex)
		return E_FAIL;
	m_pMaskTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pMaskTex)
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshEffect::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	DESC tDesc;
	if (_pArg)
	{
		tDesc = *static_cast<DESC*>(_pArg);
		m_fLifeTime = tDesc.fLifeTime;
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&tDesc.vPosition), 1.f));

	return S_OK;
}

_int CMeshEffect::Tick(_double _dDeltaTime)
{
	_int iProcess = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	return _int();
}

_int CMeshEffect::LateTick(_double _dDeltaTime)
{
	_int iProcess = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProcess)
		return iProcess;

	return _int();
}

HRESULT CMeshEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	// 쉐이더에 행렬 넘기기
	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));
	// 쉐이더에 이펙트에 필요한 값들 넘기기
	// 시간
	m_pModel->SetUp_ValueOnShader("g_fLifeTime", &m_fLifeTime, sizeof(_float));
	m_pModel->SetUp_ValueOnShader("g_fAccTime", &m_fAccTime, sizeof(_float));
	// Alpha
	m_pModel->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));
	// UV
	m_pModel->SetUp_ValueOnShader("g_isReverse", &m_isReverse, sizeof(_bool));
	m_pModel->SetUp_ValueOnShader("g_vPlusUV", &m_vPlusUV, sizeof(_float2));
	m_pModel->SetUp_ValueOnShader("g_vTiling", &m_vTiling, sizeof(_float2));
	// X
	m_pModel->SetUp_ValueOnShader("g_isFlowX", &m_isFlowX, sizeof(_bool));
	m_pModel->SetUp_ValueOnShader("g_fFlowSpeedX", &m_fFlowSpeedX, sizeof(_float));
	// Y
	m_pModel->SetUp_ValueOnShader("g_isFlowY", &m_isFlowY, sizeof(_bool));
	m_pModel->SetUp_ValueOnShader("g_fFlowSpeedY", &m_fFlowSpeedY, sizeof(_float));
	//Color
	m_pModel->SetUp_ValueOnShader("g_isCustomColor", &m_isCustomColor, sizeof(_float));
	m_pModel->SetUp_ValueOnShader("g_vColor", &m_vColor, sizeof(_float3));

	return S_OK;
}

void CMeshEffect::Free()
{
	__super::Free();

	Safe_Release(m_pModel);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pTexture);
	Safe_Release(m_pNormalTex);
	Safe_Release(m_pNoiseTex);
	Safe_Release(m_pMaskTex);
}

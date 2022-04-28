#include "pch.h"
#include "DamageFont.h"

CDamageFont::CDamageFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CDamageFont::CDamageFont(const CDamageFont& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CDamageFont::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageFont::NativeConstruct(_uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
	{
		DESC tDesc = *static_cast<DESC*>(_pArg);
		m_vPos = tDesc.vPos;
		m_fDamage = tDesc.fDamage;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CDamageFont::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 2.f, 0.f, 1.f });


	return _int();
}

_int CDamageFont::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//빌보드
	_matrix smatView;
	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, smatView.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_UP, smatView.r[1]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, smatView.r[2]);

	_vector svPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_vector svDist = svPos - smatView.r[3];
	_float fDistToUI = XMVectorGetX(XMVector3Length(svDist));

	_vector svScale = XMVectorZero();
	svScale = { fDistToUI * 0.06f, fDistToUI * 0.06f, 1.f, 1.f };
	//svScale = { fDistToUI * 0.1f, fDistToUI * 0.1f, 1.f, 1.f };
	m_pTransform->Scaling(svScale);

	for (_uint i = 0; i < m_iFontSize; ++i)
	{
		m_vecWorldTransforms[i]->Set_WorldMatrix(m_vecLocalTransforms[i]->Get_WorldMatrix() * m_pTransform->Get_WorldMatrix());
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);

	return _int();
}

HRESULT CDamageFont::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	_uint iColumn = 2;
	_uint iRow = 10;
	_uint iIndex[3] = { 5, 9, 7 };
	for (_uint i = 0; i < m_iFontSize; ++i)
	{
		_matrix smatWorld = XMMatrixTranspose(m_vecWorldTransforms[i]->Get_WorldMatrix());
		m_vecFonts[i].first->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(XMMATRIX));
		m_vecFonts[i].first->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(XMMATRIX));
		m_vecFonts[i].first->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(XMMATRIX));
		m_vecFonts[i].first->SetUp_TextureOnShader("g_DiffuseTexture", m_vecFonts[i].second);

		m_vecFonts[i].first->SetUp_ValueOnShader("g_iIndex", &m_vecIndex[i], sizeof(_uint));
		m_vecFonts[i].first->SetUp_ValueOnShader("g_iIndex", &iIndex[i], sizeof(_uint));
		m_vecFonts[i].first->SetUp_ValueOnShader("g_iColumn", &iColumn, sizeof(_uint));
		m_vecFonts[i].first->SetUp_ValueOnShader("g_iRow", &iRow, sizeof(_uint));

		m_vecFonts[i].first->Render(5);
	}

	return S_OK;
}

HRESULT CDamageFont::Ready_Components()
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vPos), 1.f));

	_uint iDamage = (_uint)m_fDamage;
	while (iDamage != 0)
	{
		iDamage = iDamage / 10;
		++m_iFontSize;
	}
	m_vecIndex.resize(m_iFontSize);

	_float fValue = (m_iFontSize - 1) * 0.5f;
	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"Texture_DamageFont");
	for (_uint i = 0; i < m_iFontSize; ++i)
	{
		// 버퍼와 텍스처
		CVIBuffer_Rect* pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
		// 로컬 트랜스폼
		CTransform* pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
		//_vector svLocalPos = { (_float)i - (0.5f * (m_iFontSize)), 0.f, 0.f, 1.f };
		_vector svLocalPos = { (_float)(i * 0.52f) - (0.5f * (m_iFontSize)), 0.f, 0.f, 1.f };
		pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalPos);
		// 월드 트랜스폼 : 로컬 * 부모 값을 넣어서 출력할 찐 트랜스폼
		CTransform* pWorldTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

		_int iIndex = (_uint)m_fDamage % (_uint)pow(10, i + 1) / (_uint)pow(10, i);
		iIndex = 0 == iIndex ? 9 : --iIndex;

		m_vecFonts.emplace_back(pBuffer, m_pTexture);
		m_vecIndex[m_iFontSize - (i + 1)] = (iIndex);
		m_vecLocalTransforms.emplace_back(pLocalTransform);
		m_vecWorldTransforms.emplace_back(pWorldTransform);
	}

	return S_OK;
}

CDamageFont* CDamageFont::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CDamageFont* pInstance = new CDamageFont(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CDamageFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDamageFont::Clone(const _uint _iSceneID, void* _pArg)
{
	CDamageFont* pInstance = new CDamageFont(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("Failed to Creating Clone CDamageFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDamageFont::Free()
{
	__super::Free();

	for (auto& font : m_vecFonts)
	{
		Safe_Release((font).first);
	}
	m_vecFonts.clear();
	Safe_Release(m_pTexture);

	for (auto& pLocal : m_vecLocalTransforms)
		Safe_Release(pLocal);
	m_vecLocalTransforms.clear();
	for (auto& pWorld : m_vecWorldTransforms)
		Safe_Release(pWorld);
	m_vecWorldTransforms.clear();
}

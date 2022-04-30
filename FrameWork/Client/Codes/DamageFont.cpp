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
		m_isCritical = tDesc.isCritical;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vScale = { 0.2f, 0.2f };
	return S_OK;
}

_int CDamageFont::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	m_fAccTime += (_float)_dDeltaTime;

	m_vScale.x -= (_float)_dDeltaTime * 0.4f;
	if (0.06f > m_vScale.x)
		m_vScale.x = 0.06f;
	m_vScale.y -= (_float)_dDeltaTime * 0.4f;
	if (0.06f > m_vScale.y)
		m_vScale.y = 0.06f;

	if (0.5f < m_fAccTime)
	{
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		svPos += _vector{ 0.f, 1.f, 0.f, 0.f } * (_float)_dDeltaTime * 0.5f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, svPos);

		m_fAlpha -= (_float)_dDeltaTime;
	}

	return _int();
}

_int CDamageFont::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (0.f >= m_fAlpha)
	{
		Set_Remove(true);
		return NO_EVENT;
	}

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
	svScale = { fDistToUI * m_vScale.x, fDistToUI * m_vScale.y, 1.f, 1.f };
	//svScale = { fDistToUI * 0.1f, fDistToUI * 0.1f, 1.f, 1.f };
	m_pTransform->Scaling(svScale);

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	return _int();
}

HRESULT CDamageFont::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float weight = 0.5f;

	// BG
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	_matrix smatWorld = XMMatrixTranspose(m_pBGLocal->Get_WorldMatrix() * m_pTransform->Get_WorldMatrix());
	_bool isCustomColor = false;
	_float3 vColor = { 0.f, 0.f, 0.f };
	m_pBG->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(XMMATRIX));
	m_pBG->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(XMMATRIX));
	m_pBG->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(XMMATRIX));
	m_pBG->SetUp_ValueOnShader("g_Alpha", &m_fAlpha, sizeof(_float));
	m_pBG->SetUp_ValueOnShader("g_isCustomColor", &isCustomColor, sizeof(_bool));
	m_pBG->SetUp_ValueOnShader("g_vColor", &vColor, sizeof(_float3));
	m_pBG->SetUp_TextureOnShader("g_DiffuseTexture", m_pBGTex);
	
	m_pBG->Render(6);

	// Fonts
	_uint iColumn = 1;
	_uint iRow = 10;
	for (_uint i = 0; i < m_iFontSize; ++i)
	{
		smatWorld = XMMatrixTranspose(m_vecLocalTransforms[i]->Get_WorldMatrix() * m_pTransform->Get_WorldMatrix());
		m_vecFonts[i]->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(XMMATRIX));
		m_vecFonts[i]->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(XMMATRIX));
		m_vecFonts[i]->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(XMMATRIX));
		m_vecFonts[i]->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);
		m_vecFonts[i]->SetUp_TextureOnShader("g_MaskTexture", m_pMaskTex);

		m_vecFonts[i]->SetUp_ValueOnShader("g_iIndex", &m_vecIndex[i], sizeof(_uint));
		m_vecFonts[i]->SetUp_ValueOnShader("g_iColumn", &iColumn, sizeof(_uint));
		m_vecFonts[i]->SetUp_ValueOnShader("g_iRow", &iRow, sizeof(_uint));
		m_vecFonts[i]->SetUp_ValueOnShader("g_Alpha", &m_fAlpha, sizeof(_float));
		m_vecFonts[i]->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float));
		m_vecFonts[i]->SetUp_ValueOnShader("g_isCritical", &m_isCritical, sizeof(_bool));

		m_vecFonts[i]->Render(5);
	}

	return S_OK;
}

HRESULT CDamageFont::Ready_Components()
{
	uniform_int_distribution<_uint> iRange(1, 10);
	_uint iRandom = iRange(g_random);

	if (7 == iRandom)
	{
		m_fDamage *= 1.5f;
		m_isCritical = true;
	}
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vPos), 1.f));
	_uint iDamage = (_uint)m_fDamage;
	while (iDamage != 0)
	{
		iDamage = iDamage / 10;
		++m_iFontSize;
	}

	/* For.BackGround */
	m_pBG = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
	m_pBGTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	//m_pBGTex->Change_Texture(L"Texture_Tuto_Base");
	m_pBGTex->Change_Texture(L"Texture_DamageFontBG2");
	m_pBGLocal = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");
	m_pBGLocal->Scaling(_vector{ (_float)m_iFontSize, 1.f, 1.f ,0.f });
	m_pBGLocal->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });

	/* For.Fonts */
	m_vecIndex.resize(m_iFontSize);

	_float fValue = (m_iFontSize - 1) * 0.5f;
	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pTexture->Change_Texture(L"Texture_DamageFont");
	m_pMaskTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	m_pMaskTex->Change_Texture(L"Texture_GreenGradi");
	for (_uint i = 0; i < m_iFontSize; ++i)
	{
		// 버퍼와 텍스처
		CVIBuffer_Rect* pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
		// 로컬 트랜스폼
		CTransform* pLocalTransform = g_pGameInstance->Clone_Component<CTransform>(0, L"Proto_Component_Transform");

		switch (m_iFontSize % 2)
		{
		case 0:
			if (m_iFontSize * 0.5f > ((_float)i + 0.5f))
			{
				_vector svLocalPos = { -(0.25f * (m_iFontSize - (i + 1))), 0.f, 0.f, 1.f };
				pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalPos);
			}
			else
			{
				_vector svLocalPos = { (0.25f * (m_iFontSize - i)), 0.f, 0.f, 1.f };
				pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalPos);
			}
			break;
		case 1:
			if (m_iFontSize * 0.5f > ((_float)i + 0.5f))
			{
				_vector svLocalPos = { -(0.25f * (m_iFontSize - (i + 1))), 0.f, 0.f, 1.f };
				pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalPos);
			}
			else if (m_iFontSize * 0.5f < ((_float)i + 0.5f))
			{
				_vector svLocalPos = { (0.25f * i), 0.f, 0.f, 1.f };
				pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalPos);
			}
			break;
		}

		// 인덱스 설정
		_int iIndex = (_uint)m_fDamage % (_uint)pow(10, i + 1) / (_uint)pow(10, i);
		iIndex = 0 == iIndex ? 9 : --iIndex;

		m_vecFonts.emplace_back(pBuffer);
		m_vecIndex[m_iFontSize - (i + 1)] = (iIndex);
		m_vecLocalTransforms.emplace_back(pLocalTransform);
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
		Safe_Release(font);
	}
	m_vecFonts.clear();
	Safe_Release(m_pTexture);
	Safe_Release(m_pMaskTex);

	for (auto& pLocal : m_vecLocalTransforms)
		Safe_Release(pLocal);
	m_vecLocalTransforms.clear();

	Safe_Release(m_pBG);
	Safe_Release(m_pBGTex);
	Safe_Release(m_pBGLocal);
}

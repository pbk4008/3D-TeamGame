#include "Actor.h"
#include "Transform.h"
#include "Effect.h"
#include "Light.h"
#include "GameInstance.h"

CActor::CActor()
	:m_fSpeed(0.f)
	, m_bDead(false)
	, m_IsAttack(false)
	, m_fMaxHp(0.f)
	, m_fCurrentHp(0.f)
	, m_fGroggyGauge(0.f)
	, m_fMaxGroggyGauge(0.f)
	, m_bGroggy(false)
	, m_fAccNoDamageTime(0.f)
{
}

CActor::CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
	, m_fSpeed(0.f)
	, m_bDead(false)
	, m_IsAttack(false)
	, m_fMaxHp(0.f)
	, m_fCurrentHp(0.f)
	, m_fGroggyGauge(0.f)
	, m_fMaxGroggyGauge(0.f)
	, m_bGroggy(false)
	, m_fAccNoDamageTime(0.f)
{
}

CActor::CActor(const CActor& rhs)
	: CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_bDead(rhs.m_bDead)
	, m_IsAttack(rhs.m_IsAttack)
	, m_fMaxHp(rhs.m_fMaxHp)
	, m_fCurrentHp(rhs.m_fCurrentHp)
	, m_fGroggyGauge(rhs.m_fGroggyGauge)
	, m_fMaxGroggyGauge(rhs.m_fMaxGroggyGauge)
	, m_bGroggy(rhs.m_bGroggy)
	, m_fAccNoDamageTime(0.f)
{
}

HRESULT CActor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CActor::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_tAttackDesc.pOwner = this;
	m_tAttackDesc.pHitObject = this;

	m_lightdesc = g_pGameInstance->Get_LightDesc(0);

	m_dissolveTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_dissolveTex->Change_Texture(L"DissovleBase"))) MSGBOX("Failed to Change Texture DissovleTex");
	m_dissolveGradientTex = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (FAILED(m_dissolveGradientTex->Change_Texture(L"DissovleGradient"))) MSGBOX("Failed to Change Texture DissovleTex");

	LIGHTDESC LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 5.f;
	LightDesc.vDiffuse = _float4(0.3686f, 04941.f, 0.60784f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.bactive = false;
	LightDesc.vPosition = _float3(0, 0, 0);

	m_LightRange = LightDesc.fRange;
	m_OrigLightRange = LightDesc.fRange;

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc, &m_pActiveLight))) MSGBOX("Failed To Adding PointLight");

	m_bUIShow = false;

	m_rimtime = m_rimtimer;

	return S_OK;
}

_int CActor::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

_int CActor::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CActor::Render()
{
	//if (FAILED(__super::Render()))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

const _float CActor::Get_HpRatio()
{
	_float fRatio = m_fCurrentHp / m_fMaxHp;

	return fRatio;
}

const _float CActor::Get_GroggyGaugeRatio()
{
	return m_fGroggyGauge / m_fMaxGroggyGauge;
}

const ATTACKDESC& CActor::Get_AttackDesc() const
{
	return m_tAttackDesc;
}

HRESULT CActor::Set_SpawnPosition(const _float3 vPoint)
{
	if (!m_pTransform)
		return E_FAIL;

	_vector vPos = XMLoadFloat3(&vPoint);
	vPos=XMVectorSetW(vPos, 1.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CActor::Set_SpawnPosition(_fvector vPos)
{
	if (!m_pTransform)
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CActor::BindConstantBuffer(const wstring& camTag, SCB* bindbuffer, RIM* rimbuffer, MOTIONBLUR* motionbuffer)
{
	if (m_pTransform == nullptr)
		MSGBOX("Failed To Apply Actor Transform nullptr");

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(camTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(camTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (bindbuffer)
	{
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_Metalic", &bindbuffer->metalic, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_Roughness", &bindbuffer->roughness, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_AO", &bindbuffer->ao, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_color", &bindbuffer->color, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if(FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &bindbuffer->empower, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	}

	if (rimbuffer)
	{
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimlightcheck", &rimbuffer->rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimintensity", &rimbuffer->rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimcolor", &rimbuffer->rimcol, sizeof(_float3)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_camdir", &rimbuffer->camdir, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	}

	if (motionbuffer)
	{
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_RotationMat", &XMMatrixTranspose(XMLoadFloat4x4(&motionbuffer->RotationMat)), sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_PreWorldViewProj", &XMMatrixTranspose(XMLoadFloat4x4(&motionbuffer->preWorldViewPorjMat)), sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	}

	return S_OK;
}

HRESULT CActor::BindLightBuffer()
{
	_matrix view, porj;
	_float3 lightpos = m_lightdesc->vPosition;
	view = XMMatrixTranspose(m_lightdesc->mLightView);
	porj = XMMatrixTranspose(m_lightdesc->mLightProj);

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightView", &view, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor LightConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightProj", &porj, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor LightConstantBuffer");
	if(FAILED(m_pModel->SetUp_ValueOnShader("g_LightPos", &lightpos, sizeof(_float3)))) MSGBOX("Failed To Apply Actor LightConstantBuffer");

	return S_OK;
}

void CActor::Active_Effect(_uint iEffectIndex)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		_vector Mypos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		Mypos = XMVectorSetY(Mypos, XMVectorGetY(Mypos) + 1.f);
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
}

void CActor::Active_Effect(_uint iEffectIndex, _fvector vPivot)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		_vector Mypos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		Mypos += vPivot;
		pEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, Mypos);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
	
}

void CActor::Active_Effect_Target(_uint iEffectIndex, _matrix TargetMat)
{
	CEffect* pEffect = g_pGameInstance->Get_Effect(iEffectIndex);
	if (!pEffect)
	{
		//MSGBOX("Effect Null!!");
		return;
	}
	if (nullptr != pEffect)
	{
		TargetMat.r[3] = XMVectorSetY(TargetMat.r[3], XMVectorGetY(TargetMat.r[3]) + 1.f);
		pEffect->Get_Transform()->Set_WorldMatrix(TargetMat);
		pEffect->setActive(true);
		pEffect->Set_Reset(true);
	}
}

void CActor::Check_NoDamage(_double dDeltaTime)
{
	if (m_isNoDamage)
	{
		m_fAccNoDamageTime += (_float)dDeltaTime;
		if (m_fAccNoDamageTime > 8.f)
		{
			m_fAccNoDamageTime = 0.f;
			m_isNoDamage = false;
			m_rimcheck = true;
		}
	}
}


void CActor::Set_AttackDesc(const ATTACKDESC& _tAttackDesc)
{
	m_tAttackDesc = _tAttackDesc;
}

void CActor::Mul_AttackDesc_Damage(const _float _fValue)
{
	m_tAttackDesc.fDamage *= _fValue;
}

void CActor::Div_AttackDesc_Damage(const _float _fValue)
{
	m_tAttackDesc.fDamage /= _fValue;
}

void CActor::Set_FootPosition(const _float3& _vPos)
{
}

void CActor::RimlightCheck(_bool check, _float3 color, _float rimtime)
{
	m_rimcheck = check;
	m_rimcol = color;
	m_rimtime = rimtime;
	m_rimtimer = rimtime;

	if (check == false)
		m_rimtime = m_rimtimer;
}

void CActor::RimIntensity(_float time)
{
	if (m_rimcheck == true)
	{
		m_rimtime += time;
		if (m_rimtime <= 0.f)
		{
			m_rimtime = m_rimtimer;
			m_rimcheck = false;
		}
	}

}

HRESULT CActor::DissolveOn(_float dissolveSpeed)
{
	m_lifetime += (g_fDeltaTime * dissolveSpeed);
	if (m_lifetime >= 1.f)
	{
		m_lifetime = 1.f;
	}
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_dissolvetime", &m_lifetime, sizeof(_float)))) MSGBOX("Failed to Apply dissolvetime");
	if (FAILED(m_pModel->SetUp_TextureOnShader("g_DissolveTex", m_dissolveTex, 0))) MSGBOX("Failed to Apply dissolveTex");
	if (FAILED(m_pModel->SetUp_TextureOnShader("g_DissolveGrTex", m_dissolveGradientTex, 0))) MSGBOX("Failed to Apply dissolveTex");

	return S_OK;
}

void CActor::LightOnOff(_fvector pos, _fvector color, _float deltaspeed)
{
	if (m_bLightCheck == true)
	{
		m_LightRange += g_fDeltaTime * -deltaspeed;

		m_pActiveLight->Set_Range(m_LightRange);
		m_pActiveLight->Set_Pos(pos);
		m_pActiveLight->Set_Color(color);

		if (m_LightRange <= 0.f)
		{
			m_LightRange = m_OrigLightRange;
			m_pActiveLight->Set_Active(false);
		}
	}
}

void CActor::Set_LightCheck(_bool check)
{
	m_bLightCheck = check;
	m_pActiveLight->Set_Active(check);
}

void CActor::Set_LightOrigRange(_float range)
{
	m_OrigLightRange = range;
	m_LightRange = range;
}

void CActor::Set_LightAmbientSpecular(_float4 ambient, _float4 specular)
{
	m_pActiveLight->Set_Ambient(ambient);
	m_pActiveLight->Set_Sepcular(specular);
}

void CActor::Hit(const ATTACKDESC& _tAttackDesc)
{
}

void CActor::Parry(const PARRYDESC& _tParryDesc)
{
}

void CActor::Execution()
{
}

void CActor::Execution(CActor* _pOther, CHierarchyNode* _pFixedBone)
{
}

void CActor::Free()
{
	CGameObject::Free();

	Safe_Release(m_pModel);
	Safe_Release(m_dissolveTex);
	Safe_Release(m_dissolveGradientTex);
}

#include "pch.h"
#include "DropObject.h"
#include "ItemData.h"
#include "Client_Function.h"
#include "SplineCurve.h"
#include "InventoryData.h"
#include "MotionTrail.h"

CDropObject::CDropObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CInteractableObject(_pDevice, _pDeviceContext)
	, m_pSplineCurve(nullptr)
{
}

CDropObject::CDropObject(const CDropObject& _rhs)
	:CInteractableObject(_rhs)
	, m_pSplineCurve(_rhs.m_pSplineCurve)
{
	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");
}

HRESULT CDropObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CDropObject::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pPlayer = *g_pGameInstance->getObjectList(_iSceneID, L"Layer_Silvermane")->begin();
	assert(m_pPlayer);
	
	m_bDrop = false;
	m_bTakable = false;
	m_pSplineCurve = new CSplineCurve();

	m_pTransform->Scaling(_vector{ 2.f, 2.f, 2.f });

	for (_int i = 0; i < 20; ++i)
	{
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)m_iSceneID, L"Layer_DropMotionTrail", L"Proto_GameObject_MotionTrail")))
			return E_FAIL;

		list<CGameObject*>* pobjlist = nullptr;
		pobjlist = g_pGameInstance->getObjectList((_uint)m_iSceneID, L"Layer_DropMotionTrail");
		CGameObject* pobj = pobjlist->back();
		pobj->setActive(false);
		static_cast<CMotionTrail*>(pobj)->Set_Model(nullptr,nullptr, m_pModel);

		m_vecMotionTrail.emplace_back(pobj);
	}

	return S_OK;
}

_int CDropObject::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;


	if (IsDrop() && false == IsTakable())
	{
		m_elapsed += (_float)_dDeltaTime;
		if (m_elapsed >= m_dropDurtaion)
		{
			SetTakableState(true);
			_vector Pos = { -0.04f, -0.1f, 0.f, 0.f };
			Active_Effect((_uint)EFFECT::ITEM, Pos);
		}
		_vector point = m_pSplineCurve->GetPoint(m_elapsed / m_dropDurtaion);
		point = XMVectorSetW(point, 1.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, point);
	}
	else if (true == IsTakable())
	{
		m_bLateTick = true;
	}

	m_pTransform->Rotation_Axis(CTransform::STATE_UP, (_float)_dDeltaTime * 3.f);


	if (m_bTakable == false)
	{
		m_MotiontrailTime += g_fDeltaTime;
		if (m_MotiontrailTime >= 0.1f)
		{
			m_MotiontrailTime = 0.f;
			m_MotionTrailIdx++;
			Create_MotionTrail(m_MotionTrailIdx, true, false);
		}

		if (m_MotionTrailIdx >= 19)
			m_MotionTrailIdx = 0;
	}

	return _int();
}

_int CDropObject::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

HRESULT CDropObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))	MSGBOX("Failed to Apply DropObject Value");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))		MSGBOX("Failed to Apply DropObject Value");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))		MSGBOX("Failed to Apply DropObject Value");

	_float4 color = _float4(0.f, 0.5529f,0.3843f,1.f);
	_float empowr = 0.1f;

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_color", &color, sizeof(_float4))))		MSGBOX("Failed to Apply DropObject Value");
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_empower", &empowr, sizeof(_float))))	MSGBOX("Failed to Apply DropObject Value");

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pModel->Render(i, 2)))		MSGBOX("Failed to Rendering DropObject Model");
	}


	return S_OK;
}

HRESULT CDropObject::Ready_Components()
{
	/* for. Transform Com */
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_DropObject", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	return S_OK;
}

_bool CDropObject::IsInteractable(void)
{
	if (false == IsDrop())
		return false;
	if (false == IsTakable())
		return false;
	if (nullptr == m_pPlayer)
		return false;

	_float dist = MathUtils::Length(m_pPlayer, this);

	if (dist <= m_interactDist)
		return true;

	return false;
}

_bool CDropObject::HasInteractionUI(void)
{
	return false;
}

_float CDropObject::GetRatioForUI(void)
{
	return 0.0f;
}

_vector CDropObject::GetGuideUIPos(void)
{
	return _vector();
}

void CDropObject::BeginInteraction(void)
{
	__super::BeginInteraction();
	Take();
}

void CDropObject::UpdateInteraction(_double _dTimeDelta)
{
	__super::UpdateInteraction(_dTimeDelta);
}

void CDropObject::EndInteraction(void)
{
	__super::EndInteraction();
}

void CDropObject::Drop(CItemData itemData, _fvector dropPos, EScatterType scatterType, CTransform* pSender)
{
	setActive(true);
	m_bDrop = true;
	m_bTakable = false;
	XMStoreFloat4(&m_dropPos, dropPos);
	m_droppedItem = itemData;
	m_pTransform->Set_State(CTransform::STATE_POSITION, dropPos);

	if (EScatterType::Quad == scatterType)
	{
		_vector pMin = XMVectorSet((m_dropPos.x - m_scatteredRadius), m_dropPos.y , (m_dropPos.z - m_scatteredRadius), 1.0f);
		_vector pMax = XMVectorSet((m_dropPos.x + m_scatteredRadius), m_dropPos.y, (m_dropPos.z + m_scatteredRadius), 1.0f);

		_vector Temp = XMLoadFloat4(&m_scatteredPos);
		MathUtils::GetRandomVector(&Temp, &pMin, &pMax);

		XMStoreFloat4(&m_scatteredPos, Temp);
	}
	else if (EScatterType::Cone == scatterType)
	{
		_vector direction = { 0.f, 0.f, 1.f, 0.f };


		_float rotAngle = (_float)MathUtils::ReliableRandom(-1.0, 1.0);
		_matrix coneMat = XMMatrixIdentity();

		coneMat = XMMatrixRotationAxis(_fvector{ 0.f, 1.f, 0.f, 0.f }, rotAngle);
		direction = XMVector3TransformNormal(XMVector4Normalize(pSender->Get_State(CTransform::STATE_LOOK)), coneMat);

		_float4 temp;
		XMStoreFloat4(&temp, direction);
		_float factor = (_float)MathUtils::ReliableRandom(2, m_scatteredRadius);
		XMStoreFloat4(&m_scatteredPos, XMLoadFloat4(&m_dropPos) + (direction * factor));
	}

	m_elapsed = 0.0f;

	_vector dir = MathUtils::Direction(XMLoadFloat4(&m_dropPos), XMLoadFloat4(&m_scatteredPos));

	// 회전관련 아직 보류
	m_pTransform->LookAt_RotYAxis(dir);

	/* for. Spline Curve */
	_vector p0;
	_vector p1 = XMLoadFloat4(&m_dropPos);	    /* start */
	_vector p2;								    /* curve */
	_vector p3 = XMLoadFloat4(&m_scatteredPos); /* end */
	_vector p4;

	_float dist = MathUtils::Length(p3, p1);
	_float curveFactor = dist * 1.0f;
	_vector temp = p3 - p1;
	temp /= 2;
	temp = temp + (
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)) * 0.f +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * curveFactor +
		XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * 0.f);
	p2 = m_pTransform->Get_State(CTransform::STATE_POSITION) + temp;
	p0 = p1 + (MathUtils::Direction(p2, p1));
	p4 = p3 + (MathUtils::Direction(p2, p3));

	m_pSplineCurve->DeletePoints();
	m_pSplineCurve->AddPoint(p0);
	m_pSplineCurve->AddPoint(p1);
	m_pSplineCurve->AddPoint(p2);
	m_pSplineCurve->AddPoint(p3);
	m_pSplineCurve->AddPoint(p4);
}

void CDropObject::Take(void)
{
	m_bDrop = false;
	assert(m_droppedItem.ItemType != EItemType::NONE);

	if (m_droppedItem.ItemType == EItemType::Resorce)
	{
		/* Add Inventory Resource Data */ 
	}
	else if (m_droppedItem.ItemType == EItemType::Equipment)
	{
		/* Inventory push Item */
 		m_pInventoryData->PushItem(m_droppedItem);

		_vector pivot = { 0.f, -0.05f, 0.f, 0.f };
		Active_Effect((_uint)EFFECT::EAT_ITEM, pivot);

		m_bInteractDead = true;
		m_bTakable = false;
	}
	else if (m_droppedItem.ItemType == EItemType::Potion)
	{
		if (true == m_pInventoryData->PushPotion())
		{
			m_bInteractDead = true;
			m_bTakable = false;
		}
		else
		{
			m_bInteractDead = false;
			m_bTakable = true;
			m_bDrop = true;
		}
	}
}

void CDropObject::SetTakableState(_bool bState)
{
	m_bTakable = bState;
}

_bool CDropObject::IsDrop(void)
{
	return m_bDrop;
}

_bool CDropObject::IsTakable(void)
{
	return m_bTakable;
}

HRESULT CDropObject::Create_MotionTrail(_int idex, _bool runcheck, _bool throwcheck)
{
	if (idex <= 19)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();

		_matrix smatWorld;

		smatWorld = m_pTransform->Get_WorldMatrix();
		_vector position, camposition;
		position = m_pTransform->Get_State(CTransform::STATE_POSITION);
		camposition = g_pGameInstance->Get_CamPosition(wstrCamTag);

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->setActive(true);
		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_BoneMat(m_pModel->Get_CurBoneMatrix());
		_float uvdvid = 0.f;
		if (runcheck == true)
			uvdvid = 0.3f;
		else
			uvdvid = 0.9f;

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_Info(XMMatrixIdentity()
			, XMMatrixIdentity()
			, m_pTransform->Get_WorldMatrix()
			, uvdvid);

		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_RunCheck(runcheck);
		static_cast<CMotionTrail*>(m_vecMotionTrail[idex])->Set_ThrowCheck(throwcheck);
	}

	return S_OK;
}

CDropObject* CDropObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CDropObject* pInstance = new CDropObject(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CDropObject Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDropObject::Clone(const _uint _iSceneID, void* _pArg)
{
	CDropObject* pInstance = new CDropObject(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CDropObject Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDropObject::Free()
{
	__super::Free();

	Safe_Delete(m_pSplineCurve);
	m_pInventoryData = nullptr;

}
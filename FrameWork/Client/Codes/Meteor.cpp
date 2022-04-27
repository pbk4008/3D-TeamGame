#include "pch.h"
#include "Meteor.h"
#include "Animation.h"

CMeteor::CMeteor()
	: m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_fSpeed(0.f)
	, m_bRemoveCheck(false)
	, m_fRandSpawnTime(0.f)
	, m_fAccTime(0.f)
	, m_bStart(false)
	, m_fAccRotateTime(0.f)
	, m_fPreY(0.f)
	, m_fAccGravityTime(0.f)
	, m_fAccMotionTim(0.f)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
	ZeroMemory(&m_vRandNorm, sizeof(_float4));
}

CMeteor::CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	,m_pStaticModel(nullptr)
	, m_pAnimModel(nullptr)
	, m_pCollider(nullptr)
	, m_fSpeed(0.f)
	, m_bRemoveCheck(false)
	, m_fRandSpawnTime(0.f)
	, m_fAccTime(0.f)
	, m_bStart(false)
	, m_fAccRotateTime(0.f)
	, m_fPreY(0.f)
	, m_fAccGravityTime(0.f)
	, m_fAccMotionTim(0.f)
{
	ZeroMemory(&m_vDestination, sizeof(_float4));
	ZeroMemory(&m_vRandNorm, sizeof(_float4));
}

CMeteor::CMeteor(const CMeteor& rhs)
	: CGameObject(rhs)
	, m_pStaticModel(rhs.m_pStaticModel)
	, m_pAnimModel(rhs.m_pAnimModel)
	, m_pCollider(rhs.m_pCollider)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDestination(rhs.m_vDestination)
	, m_bRemoveCheck(rhs.m_bRemoveCheck)
	, m_fRandSpawnTime(rhs.m_fRandSpawnTime)
	, m_fAccTime(rhs.m_fAccTime)
	, m_bStart(false)
	, m_fAccRotateTime(0.f)
	, m_vRandNorm(rhs.m_vRandNorm)
	, m_fPreY(0.f)
	, m_fAccGravityTime(0.f)
	, m_fAccMotionTim(0.f)
{
	Safe_AddRef(m_pStaticModel);
	Safe_AddRef(m_pAnimModel);
	Safe_AddRef(m_pCollider);
}

HRESULT CMeteor::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_METEOR;

	return S_OK;
}

HRESULT CMeteor::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CGameObject::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimModel->SetUp_AnimationIndex(0);

	return S_OK;
}

_int CMeteor::Tick(_double _dDeltaTime)
{
	/*if (!m_bStart)
	{
		m_fAccTime += (_float)_dDeltaTime;
		if (m_fAccTime >= m_fRandSpawnTime)
		{
			m_bStart = true;
			m_fAccTime = 0.f;
			m_fRandSpawnTime = 0.f;
		}
		else
			return 0;
	}*/
	m_pCollider->Tick(_dDeltaTime);
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float fY = XMVectorGetY(vPos);
	if (fY < -30.f)
	{
		m_pCollider->Reset_Power();
		setActive(false);
		m_fAccGravityTime = 0.f;
	}

	m_fAccRotateTime += (_float)_dDeltaTime;
	m_pCollider->Add_Torque(XMLoadFloat4(&m_vRandNorm), (_float)_dDeltaTime);


	if (fY > 0)
	{
		if (m_fPreY < fY)
			m_fPreY = fY;
		else
		{
			_vector vPow = XMVectorSet(0.f, (_float)_dDeltaTime*-50 ,0.f,0.f);
			m_pCollider->Add_Force(vPow);
			m_fAccGravityTime += (_float)_dDeltaTime;
			if (m_fAccGravityTime > 0.5f)
				m_fAccGravityTime = 0.f;
		}
	}
	if (fY < -30.f)
	{
		m_pCollider->Reset_Power();
		setActive(false);
		m_fAccGravityTime = 0.f;
	}
	
	if (m_bRemoveCheck)
	{
		m_pTransform->Scaling(XMVectorSet(0.15f, 0.15f, 0.15f, 0.f));
		m_pTransform->SetUp_Rotation(_float3(0.f, 0.f, 0.f));
		m_pAnimModel->Update_CombinedTransformationMatrix(_dDeltaTime*3.f);
		vector<CAnimation*> pVecAnim = m_pAnimModel->Get_Animations();

		if (pVecAnim[0]->Is_Finished())
		{
			m_pTransform->Scale_One();
			pVecAnim[0]->Reset_Animation();
			m_pCollider->Reset_Power();
			m_fAccGravityTime = 0.f;
			setActive(false);
			m_bStart = false;
			m_bRemoveCheck = false;
		}
	}

	return _int();
}

_int CMeteor::LateTick(_double _dDeltaTime)
{
	/*if (!m_bStart)
		return 0; */
	m_pCollider->Update_Transform();
	//if (!m_bRemoveCheck)


	//if (!m_pRenderer)
	//	return E_FAIL;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (m_pRenderer->Get_RenderButton(CRenderer::VELOCITYBLUR) == true)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_VELOCITY, this)))
			return -1;
	}
	return _int();
}

HRESULT CMeteor::Render()
{
	_matrix smatWorld, smatView, smatProj;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	RIM rimDesc;
	ZeroMemory(&rimDesc, sizeof(RIM));

	rimDesc.rimcheck = true;
	rimDesc.rimcol = _float3(1.f, 0.49f, 0.f);
	rimDesc.rimintensity = 30.f;
	XMStoreFloat4(&rimDesc.camdir, XMVector3Normalize(g_pGameInstance->Get_CamPosition(L"Camera_Silvermane") - m_pTransform->Get_State(CTransform::STATE_POSITION)));

	_float4 fColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float fEmpower = 1.f;
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_color", &fColor, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_empower", &fEmpower, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_rimlightcheck", &rimDesc.rimcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_rimintensity", &rimDesc.rimintensity, sizeof(_float)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_rimcolor", &rimDesc.rimcol, sizeof(_float3)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_camdir", &rimDesc.camdir, sizeof(_float4)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	if (!m_bRemoveCheck)
	{
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix))))
			return E_FAIL;
		if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix))))
			return E_FAIL;
		for (_uint i = 0; i < m_pStaticModel->Get_NumMeshContainer(); ++i)
			if (FAILED(m_pStaticModel->Render(i, 0))) 	return E_FAIL;
	}
	return S_OK;
}

HRESULT CMeteor::Render_Velocity()
{
	wstring wstrCam = g_pGameInstance->Get_BaseCameraTag();

	_matrix matTransform = m_pTransform->Get_WorldMatrix();
	matTransform *= g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_VIEW) * g_pGameInstance->Get_Transform(wstrCam, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);

	matTransform.r[0]=XMVector3Normalize(matTransform.r[0]);
	matTransform.r[1]=XMVector3Normalize(matTransform.r[1]);
	matTransform.r[2] = XMVector3Normalize(matTransform.r[2]);

	for (_uint i = 0; i < 3; i++)
		matTransform.r[i] *= 0.5f;
	matTransform.r[3] = XMVectorZero();

	_matrix matPreWVP = g_pGameInstance->GetPreViewProtj(m_PreWroldMat);


	matTransform = XMMatrixTranspose(matTransform);
	matPreWVP = XMMatrixTranspose(matPreWVP);

	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_RotationMat", &matTransform, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");
	if (FAILED(m_pStaticModel->SetUp_ValueOnShader("g_PreWorldViewProj", &matPreWVP, sizeof(_matrix)))) MSGBOX("Failed To Apply Actor ConstantBuffer");

	for (_uint i = 0; i < m_pStaticModel->Get_NumMeshContainer(); i++)
	{
		if (FAILED(m_pStaticModel->Render(i, 2)))
			MSGBOX("Fialed To Rendering Silvermane");
	}

	m_fAccMotionTim += g_fDeltaTime;
	if (m_fAccMotionTim > 0.1f)
	{
		m_PreWroldMat = m_pTransform->Get_WorldMatrix();
		m_fAccMotionTim = 0.f;
	}

	return S_OK;
}

HRESULT CMeteor::Ready_Component()
{
	CTransform::TRANSFORMDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));
	tDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tDesc.fSpeedPerSec = 0.f;

	m_pTransform->Set_TransformDesc(tDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Meteor_Static", L"Meteor_Static", (CComponent**)&m_pStaticModel)))
		return E_FAIL;
	_matrix matIdnetity = XMMatrixIdentity();
	m_pStaticModel->Set_PivotMatrix(matIdnetity);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Meteor_Anim", L"Meteor_Anim", (CComponent**)&m_pAnimModel)))
		return E_FAIL;
	//_matrix matScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//m_pAnimModel->Set_PivotMatrix(matScale);
	

	CSphereCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 2.f;
	tColliderDesc.tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.tColliderDesc.isTrigger = true;
	tColliderDesc.tColliderDesc.isGravity = true;
	tColliderDesc.tColliderDesc.fMass = 1.f;
	tColliderDesc.tColliderDesc.isVisualization = true;
	tColliderDesc.tColliderDesc.isSceneQuery = true;
	tColliderDesc.tColliderDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", L"Collider", (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	m_pCollider->setShapeLayer((_uint)ELayer::Pot);
	m_pCollider->Remove_ActorFromScene();
	
	return S_OK;
}


_int CMeteor::Move(_fvector vPos, _uint iNum)
{	
	m_pCollider->Add_ActorToScene();
	m_pCollider->Reset_Power();
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vDir = XMVectorZero();
	_float fAngle = 0.f;
	_float fPow = 180.f;
	switch (iNum)
	{
	case 0:
		vDir = XMVectorSet(0.3f, 0.f, 1.f, 0.f);
		fAngle = 50.f;
		break;
	case 1:
		vDir = XMVectorSet(-0.5f, 0.f, 0.8f, 0.f);
		fAngle = 50.f;
		break;
	case 2:
		vDir = XMVectorSet(0.5f, 0.f, 0.8f, 0.f);
		fAngle = 45.f;
		break;
	case 3:
		vDir = XMVectorSet(0.4f, 0.f, -0.5f, 0.f);
		fAngle = 50.f;
		break;
	case 4:
		vDir = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		fAngle = 60.f;
		fPow = 180.f;
		break;
	case 5:
		vDir = XMVectorSet(-1.f, 0.f, 0.5f, 0.f);;
		fAngle = 50.f;
		break;
	}

	vDir = XMVector3Normalize(vDir);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vTmpLook = XMVector3Cross(vDir, vUp);
	_matrix matRot=XMMatrixRotationAxis(vTmpLook, XMConvertToRadians(fAngle));
	
	vDir=XMVector3TransformNormal(vDir, matRot);

	vDir *= fPow;

	m_pCollider->Add_Force(vDir);

	m_fRandSpawnTime = (_float)MathUtils::ReliableRandom(1.f, 3.f);

	_float fNormX = (_float)MathUtils::ReliableRandom(-1.0, 1.0);
	_float fNormY = (_float)MathUtils::ReliableRandom(-1.0, 1.0);
	_float fNormZ = (_float)MathUtils::ReliableRandom(-1.0, 1.0);

	m_vRandNorm = _float4(fNormX, fNormY, fNormZ,0.f);
	return _int();
}

_int CMeteor::Shot()
{
	m_pCollider->Add_ActorToScene();

	_matrix matPlayer = g_pObserver->Get_PlayerWorldMatrix();
	_vector vPos = g_pObserver->Get_PlayerPos();
	_vector vPlayerRight = matPlayer.r[0];
	vPlayerRight = XMVector3Normalize(vPlayerRight);

	cout << XMVectorGetX(vPlayerRight) << ", " << XMVectorGetY(vPlayerRight) << ", " << XMVectorGetZ(vPlayerRight)<<endl;
	_vector vCreatePos = vPos + (vPlayerRight*10.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vCreatePos);

	_vector vAxis = XMVectorSet(-1.f, 0.f, -1.f, 0.f);
	vAxis = XMVector3Normalize(vAxis);

	_matrix matRot = XMMatrixRotationAxis(vAxis, XMConvertToRadians(60.f));
	_vector vDir = XMVectorSet(-1.f, 0.f, 1.f, 0.f);
	vDir=XMVector3Normalize(vDir);
	vDir=XMVector3TransformNormal(vDir, matRot);

	/*_vector vDir = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	vDir=XMVectorSetY(vDir, 1.f);
	vDir=XMVector3Normalize(vDir);*/
	vDir *= 100.f;
	m_pCollider->Add_Force(vDir);
	
	return _int();
}

_bool CMeteor::Find_HitPlayer(vector<CGameObject*>* pVecActor)
{
	for (auto& pActor : *pVecActor)
	{
		if (pActor->getTag() == (_uint)GAMEOBJECT::PLAYER)
			return true;
	}
	return false;
}



CMeteor* CMeteor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMeteor* pInstance = new CMeteor(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMeteor Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeteor::Clone(const _uint iSceneID, void* pArg)
{
	CMeteor* pInstance = new CMeteor(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CMeteor Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor::Free()
{
	CGameObject::Free();
	Safe_Release(m_pStaticModel);
	Safe_Release(m_pAnimModel);
	Safe_Release(m_pCollider);
}

#include "CharacterController.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "ControllerFilterCallback.h"
#include "Gizmo.h"
#include "Transform.h"

CCharacterController::CCharacterController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
{
}

CCharacterController::CCharacterController(const CCharacterController& _rhs)
	: CComponent(_rhs)
	, m_pFilterCallback(_rhs.m_pFilterCallback)
{
}

HRESULT CCharacterController::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) 
		return E_FAIL;

	m_pFilterCallback = new CControllerFilterCallback();

	return S_OK;
}

HRESULT CCharacterController::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg))) 
		return E_FAIL;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	m_pGizmo = pInstance->Clone_Component<CGizmo>(0, L"Proto_Component_Gizmo");

	RELEASE_INSTANCE(CGameInstance);

	if (_pArg)
	{
		memcpy_s(&m_tCharacterControllerDesc, sizeof(CHARACTERCONTROLLERDESC), _pArg, sizeof(CHARACTERCONTROLLERDESC));
		m_pOwnerTransform = m_tCharacterControllerDesc.pGameObject->Get_Transform();
	}


	if (FAILED(Create_Controller())) 
		return E_FAIL;

	XMStoreFloat4x4(&m_matLocal, XMMatrixIdentity());





#pragma region 그리기용
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t			ShaderByteCodeLength = 0;
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);
#pragma endregion


	return S_OK;
}

const _int CCharacterController::Tick(const _double& _dDeltaTime)
{
	_matrix smatLocal = XMLoadFloat4x4(&m_matLocal);

	smatLocal *= m_pOwnerTransform->Get_CombinedMatrix();
	_matrix matScale = Update_Scale(smatLocal);
	_matrix matRotate = Update_Rotate(smatLocal);

	matScale *= matRotate;

	PxVec3 pxPos = ToPxVector(smatLocal.r[3]);
	_matrix matPos = Update_Position(pxPos);
	matScale *= matPos;

	XMStoreFloat4x4(&m_matWorld, matScale);

	return _int();
}

const _int CCharacterController::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

HRESULT CCharacterController::Render()
{
	if (!m_pGizmo)
		return E_FAIL;
	
	//CPhysicsXSystem* pPhysXSystem = GET_INSTANCE(CPhysicsXSystem);
	//const PxRenderBuffer& rb = pPhysXSystem->Get_RenderBuffer();
	//RELEASE_INSTANCE(CPhysicsXSystem);

	const PxRenderBuffer& rb = m_pPxController->getActor()->getScene()->getRenderBuffer();
	//PxU32 index = m_pPxController->getActor()->getInternalIslandNodeIndex();
	_float4 vColor = { 1.f, 0.f, 0.f, 1.f };

	m_pEffect->SetView(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	PxU32 iNBLines = rb.getNbLines();
	for (PxU32 i = 0; i < iNBLines; ++i)
	{
		const PxDebugLine line = rb.getLines()[i];
		_float3 pos0 = FromPxVec3(line.pos0);
		_float3 pos1 = FromPxVec3(line.pos1);
		_vector vPos0 = XMLoadFloat4(&_float4(pos0.x, pos0.y, pos0.z, 1.f));
		_vector vPos1 = XMLoadFloat4(&_float4(pos1.x, pos1.y, pos1.z, 1.f));

		DX::DrawTriangle(m_pBatch, vPos0, vPos1, vPos0, XMLoadFloat4(&vColor));
	}

	m_pBatch->End();

	return S_OK;
}

const CCharacterController::CHARACTERCONTROLLERDESC& CCharacterController::Get_CharacterControllerDesc() const
{
	return m_tCharacterControllerDesc;
}

PxMaterial* CCharacterController::Get_Material()
{
	return m_pMaterial;
}

void CCharacterController::Set_OwnerTransform(CTransform* _pTransform)
{
	m_pOwnerTransform = _pTransform;
}

const _bool CCharacterController::IsDown()
{
	return m_curFlag & PxControllerCollisionFlag::eCOLLISION_DOWN;
}

const _bool CCharacterController::IsUp()
{
	return m_curFlag & PxControllerCollisionFlag::eCOLLISION_UP;
}

const _bool CCharacterController::IsSides()
{
	return m_curFlag & PxControllerCollisionFlag::eCOLLISION_SIDES;
}

HRESULT CCharacterController::Create_Controller()
{
	CPhysicsXSystem* pPhysXSystem = GET_INSTANCE(CPhysicsXSystem);

	if (FAILED(pPhysXSystem->Create_Material(
		m_tCharacterControllerDesc.fStaticFriction,
		m_tCharacterControllerDesc.fDynamicFriction,
		m_tCharacterControllerDesc.fRestitution
		, &m_pMaterial)))
	{
		RELEASE_INSTANCE(CPhysicsXSystem);
		return E_FAIL;
	}

	if (FAILED(pPhysXSystem->Create_CharacterController(this, &m_pPxController, m_vecShapes)))
	{
		RELEASE_INSTANCE(CPhysicsXSystem);
		return E_FAIL;
	}
	m_pPxController->setContactOffset(PxF32(0.f));
	m_pPxController->setStepOffset(PxF32(0.f));

	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

void CCharacterController::Move(const _double& _dDeltaTime, const _float3 _vVelocity)
{
	PxVec3 pxvVelocity = ToPxVec3(_vVelocity);
	//pxvVelocity.y += -9.8f * _dDeltaTime;
	if (!pxvVelocity.isFinite())
	{
		m_vVelocity = { 0.f, 0.f, 0.f };
		pxvVelocity = { 0.f, 0.f, 0.f };
	}
	PxControllerFilters filter;
	m_preFlag = m_curFlag;
	m_curFlag = m_pPxController->move(pxvVelocity, 0.f, (_float)_dDeltaTime, filter);
}

void CCharacterController::Update_OwnerTransform()
{
	PxExtendedVec3 pxPosition = m_pPxController->getPosition();

	_float3 vPosition = { (_float)pxPosition.x, (_float)pxPosition.y, (_float)pxPosition.z };
	_vector svPosition = XMLoadFloat3(&vPosition) - XMLoadFloat3(&m_tCharacterControllerDesc.vPosition);
	m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(svPosition, 1.f));
}

_fvector CCharacterController::getQuaternion(_fmatrix matTransform)
{
	_vector vRight, vUp, vLook;
	vRight = matTransform.r[0];
	vUp = matTransform.r[1];
	vLook = matTransform.r[2];

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);

	_matrix matRotation;
	ZeroMemory(&matRotation, sizeof(_matrix));

	matRotation.r[0] = vRight;
	matRotation.r[1] = vUp;
	matRotation.r[2] = vLook;

	_vector vQuaternion = XMQuaternionRotationMatrix(matRotation);

	return vQuaternion;
}

_fvector CCharacterController::ToXMVector4(const PxQuat pxquat)
{
	_fvector vVector = XMVectorSet(pxquat.x, pxquat.y, pxquat.z, pxquat.w);

	return vVector;
}

const PxVec3 CCharacterController::ToPxVector(_fvector xmvec)
{
	PxVec3 vVector = PxVec3(XMVectorGetX(xmvec), XMVectorGetY(xmvec), XMVectorGetZ(xmvec));
	return vVector;
}

_fmatrix CCharacterController::Update_Scale(_fmatrix matTransform)
{
	_float fSizeX = XMVectorGetX(XMVector3Length(matTransform.r[0]));
	_float fSizeY = XMVectorGetX(XMVector3Length(matTransform.r[1]));
	_float fSizeZ = XMVectorGetX(XMVector3Length(matTransform.r[2]));

	PxGeometryHolder tHolder = m_vecShapes[0]->getGeometry();
	tHolder.capsule().halfHeight = fSizeY * 0.5f;
	//	tHolder.capsule().radius = fSizeX * 0.5f;
	//	m_pShape->setGeometry(tHolder.capsule());

	_matrix matScale = XMMatrixScaling(fSizeX, fSizeY, fSizeZ);

	return matScale;
}

_fmatrix CCharacterController::Update_Rotate(_fmatrix matTransform)
{
	_vector vTransformQuat = getQuaternion(matTransform);
	PxVec3 pxTransformQuat = ToPxVector(vTransformQuat);
	PxTransform pRigidTr = m_pPxController->getActor()->getGlobalPose();
	pRigidTr.rotate(pxTransformQuat);

	_vector vQuat = ToXMVector4(pRigidTr.q);
	_matrix matRotate = XMMatrixRotationQuaternion(vQuat);

	return matRotate;
}

_fmatrix CCharacterController::Update_Position(PxVec3 vPos)
{
	_float3 vOffsetPos = m_tCharacterControllerDesc.vPosition;
	vPos.x += vOffsetPos.x;
	vPos.y += vOffsetPos.y;
	vPos.z += vOffsetPos.z;
	_matrix matPos = XMMatrixIdentity();
	PxTransform pRigidTr = m_pPxController->getActor()->getGlobalPose();
	pRigidTr.p = vPos;

	_vector vMatPos = XMVectorSet(pRigidTr.p.x, pRigidTr.p.y, pRigidTr.p.z, 1.f);

	static_cast<PxRigidDynamic*>(m_pPxController->getActor())->setGlobalPose(pRigidTr);
	matPos.r[3] = vMatPos;
	return matPos;
}

CCharacterController* CCharacterController::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCharacterController* pInstance = new CCharacterController(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMesh Collider Crate Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCharacterController::Clone(void* _pArg)
{
	CCharacterController* pInstance = new CCharacterController(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CMesh Collider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCharacterController::Free()
{
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pBatch);
	Safe_Release(m_pInputLayout);

	//// 컨트롤러랑 둘중에 하나만 릴리즈 해줘야 한다
	//for (auto& pShape : m_vecShapes)
	//{
	//	if (pShape)
	//		pShape->release();
	//}
	//m_vecShapes.clear();

	if (m_pPxController) 
		m_pPxController->release();
	if (!m_isCloned)
		Safe_Delete(m_pFilterCallback);
	if(m_pMaterial)
		m_pMaterial->release();
	
	Safe_Release(m_pGizmo);
	__super::Free();
}

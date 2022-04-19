#include "CharacterController.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "ControllerFilterCallback.h"
#include "Gizmo.h"
#include "Transform.h"

CCharacterController::CCharacterController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
	, m_pPhysX(CPhysicsXSystem::GetInstance())
{
	Safe_AddRef(m_pPhysX);
}

CCharacterController::CCharacterController(const CCharacterController& _rhs)
	: CComponent(_rhs)
	, m_pPhysX(_rhs.m_pPhysX)
	, m_pFilterCallback(_rhs.m_pFilterCallback)
{
	Safe_AddRef(m_pPhysX);
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
	if (_pArg)
	{
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_pOwnerTransform = m_tDesc.pGameObject->Get_Transform();
	}

	if (FAILED(__super::NativeConstruct(_pArg))) 
		return E_FAIL;

	if (FAILED(Create_Controller())) 
		return E_FAIL;

	return S_OK;
}

const _int CCharacterController::Tick(const _double& _dDeltaTime)
{
	return _int();
}

const _int CCharacterController::LateTick(const _double& _dDeltaTime)
{
	return _int();
}

void CCharacterController::Remove_CCT()
{
	//m_pPhysX->Remove_Actor(m_pPxController->getActor());
	Safe_PxRelease(m_pPxController);
}

const CCharacterController::DESC& CCharacterController::Get_CharacterControllerDesc() const
{
	return m_tDesc;
}

void CCharacterController::setMaterial(PxMaterial* _pMateiral)
{
	m_pMaterial = _pMateiral;
}

void CCharacterController::setPxController(PxController* _pPxController)
{
	m_pPxController = _pPxController;
}

void CCharacterController::setShapes(vector<PxShape*>& _vecShapes)
{
	m_vecShapes = _vecShapes;
}

void CCharacterController::setFootPosition(const _float3 & _vPosition)
{
	if (!m_pPxController)
		return;

	PxExtendedVec3 pxvFootPos = { _vPosition.x, _vPosition.y, _vPosition.z };
	m_pPxController->setFootPosition(pxvFootPos);
}

void CCharacterController::setPosition(const _float3& _vPosition)
{
	if (!m_pPxController)
		return;

	m_pPxController->setPosition(ToPxExtendedVec3(_vPosition));
}

void CCharacterController::setOwnerTransform(CTransform* _pTransform)
{
	m_pOwnerTransform = _pTransform;
}

void CCharacterController::setShapeLayer(const _uint _iLayer)
{
	if (!m_pPhysX)
		return;

	for (auto pShape : m_vecShapes)
	{
		m_pPhysX->Set_ShapeLayer(pShape, _iLayer);
	}
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

	if (FAILED(pPhysXSystem->Create_CharacterController(this)))
	{
		RELEASE_INSTANCE(CPhysicsXSystem);
		return E_FAIL;
	}
	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

void CCharacterController::Move(const _double& _dDeltaTime, const _float3 _vVelocity)
{
	if (!m_pPxController)
		return;

	PxVec3 pxvVelocity = ToPxVec3(_vVelocity);
	if (!pxvVelocity.isFinite())
	{
		m_vVelocity = { 0.f, 0.f, 0.f };
		pxvVelocity = { 0.f, 0.f, 0.f };
	}
	PxControllerFilters filter;
	filter.mCCTFilterCallback = m_pFilterCallback;
	m_preFlag = m_curFlag;
	m_curFlag = m_pPxController->move(pxvVelocity, 0.f, (_float)_dDeltaTime, filter);
}

void CCharacterController::Update_OwnerTransform()
{
	if (!m_pPxController)
		return;

	//PxExtendedVec3 pxPosition = m_pPxController->getPosition();

	//_float3 vOffset = { (_float)pxPosition.x, (_float)pxPosition.y, (_float)pxPosition.z };
	//_vector svPosition = XMLoadFloat3(&vOffset) - XMLoadFloat3(&m_tDesc.vOffset);
	//m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(svPosition, 1.f));

	PxExtendedVec3 pxPosition = m_pPxController->getFootPosition();

	_float4 vPosition = { (_float)pxPosition.x, (_float)pxPosition.y, (_float)pxPosition.z, 1.f};
	m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));

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

CCharacterController* CCharacterController::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCharacterController* pInstance = new CCharacterController(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCharacterController Crate Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCharacterController::Clone(void* _pArg)
{
	CCharacterController* pInstance = new CCharacterController(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CCharacterController Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCharacterController::Free()
{
	__super::Free();

	//// 컨트롤러랑 둘중에 하나만 릴리즈 해줘야 한다
	//for (auto& pShape : m_vecShapes)
	//	Safe_PxRelease(pShape)
	//m_vecShapes.clear();

	Safe_PxRelease(m_pPxController);
	Safe_PxRelease(m_pMaterial);

	if (!m_isCloned)
		Safe_Delete(m_pFilterCallback);

	Safe_Release(m_pPhysX);
}

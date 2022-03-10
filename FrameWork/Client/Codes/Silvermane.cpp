#include "pch.h"
#include "..\Headers\Silvermane.h"

#include "Camera_Silvermane.h"
#include "Needle.h"

#pragma region 스테이트들
#include "Silvermane_Idle.h"

//////////////////// Jog
#include "Silvermane_JogBwd.h"
#include "Silvermane_JogBwdStart.h"
#include "Silvermane_JogBwdPivot180.h"

#include "Silvermane_JogFwd.h"
#include "Silvermane_JogFwdPivot180.h"
#include "Silvermane_JogFwdStart.h"
#include "Silvermane_JogFwdStop.h"

#include "Silvermane_JogLeft.h"
#include "Silvermane_JogLeftPivot180.h"
#include "Silvermane_JogLeftStart.h"

#include "Silvermane_JogRight.h"
#include "Silvermane_JogRightPivot180.h"
#include "Silvermane_JogRightStart.h"

//////////////////// Sprint
#include "Silvermane_SprintFwd.h"
#include "Silvermane_SprintFwdStart.h"
#include "Silvermane_SprintFwdStop.h"

//////////////////// 1H
#include "1H_SowrdAttackNormalR1_01.h"
#include "1H_SowrdAttackNormalR1_02.h"

#include "1H_SwordEquipOff.h"
#include "1H_SwordEquipOn.h"

#include "1H_SwordDodgeSpinFwd_V3.h"
#pragma endregion


CSilvermane::CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CSilvermane::CSilvermane(const CSilvermane& _rhs)
	: CActor(_rhs)
{
}

HRESULT CSilvermane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSilvermane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg))) return E_FAIL;

	if (FAILED(Ready_Components())) return E_FAIL;
	if (FAILED(Ready_States())) return E_FAIL;

	CHierarchyNode* pWeaponBone = m_pModel->Get_BoneMatrix("weapon_r");
	if (g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Weapon", L"Needle", pWeaponBone, (CGameObject**)&m_pWeapon))
		return E_FAIL;
	m_pWeapon->Set_Owner(this);
	m_pWeapon->Set_OwnerPivotMatrix(m_pModel->Get_PivotMatrix());
	Set_EquipWeapon(true);

	return S_OK;
}

_int CSilvermane::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	iProgress = m_pStateController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	iProgress = Trace_CameraLook(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	iProgress = m_pAnimationController->Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	return _int();
}

_int CSilvermane::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	iProgress = m_pStateController->LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	if(FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this))) return -1;

	return _int();
}

HRESULT CSilvermane::Render()
{
	if (FAILED(__super::Render())) return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_CombinedMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if(FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		//if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0))) return E_FAIL;
	}

#ifdef _DEBUG
	if (FAILED(m_pAnimationController->Render())) return E_FAIL;
	if (FAILED(m_pStateController->Render())) return E_FAIL;
	wstring wstrAngle = L"Angle : " + to_wstring(m_fAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrAngle.c_str(), _float2(0.f, 300.f), _float2(0.8f, 0.8f))))
		return E_FAIL;
	wstring wstrPlusAngle = L"Plus Angle : " + to_wstring(m_fPlusAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrPlusAngle.c_str(), _float2(0.f, 340.f), _float2(0.8f, 0.8f))))
		return E_FAIL;
	wstring wstrDirAngle = L"Dir Angle : " + to_wstring(m_fDirAngle);
	if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), wstrDirAngle.c_str(), _float2(0.f, 380.f), _float2(0.8f, 0.8f))))
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CSilvermane::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 10.f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Top"), 0);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Down"), 1);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Cloak"), 2);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Silvermane_Hair"), 3);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Com_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Com_StateController", L"StateController", (CComponent**)&m_pStateController)))
		return E_FAIL;
	m_pStateController->Set_GameObject(this);

	return S_OK;
}

HRESULT CSilvermane::Ready_States()
{
#pragma region Default
	if (FAILED(m_pStateController->Add_State(L"Idle", CSilvermane_Idle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwd", CSilvermane_JogBwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwdStart", CSilvermane_JogBwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogBwdPivot180", CSilvermane_JogBwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwd", CSilvermane_JogFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdPivot180", CSilvermane_JogFwdPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdStart", CSilvermane_JogFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogFwdStop", CSilvermane_JogFwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeft", CSilvermane_JogLeft::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeftPivot180", CSilvermane_JogLeftPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogLeftStart", CSilvermane_JogLeftStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRight", CSilvermane_JogRight::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRightPivot180", CSilvermane_JogRightPivot180::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"JogRightStart", CSilvermane_JogRightStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwd", CSilvermane_SprintFwd::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwdStart", CSilvermane_SprintFwdStart::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"SprintFwdStop", CSilvermane_SprintFwdStop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region 1H
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_01", C1H_SowrdAttackNormalR1_01::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordAttackNormalR1_02", C1H_SowrdAttackNormalR1_02::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordEquipOff", C1H_SwordEquipOff::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_SwordEquipOn", C1H_SwordEquipOn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pStateController->Add_State(L"1H_DodgeSpin", C1H_SwordDodgeSpinFwd_V3::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	for (auto& pair : m_pStateController->Get_States())
	{
		pair.second->Set_StateController(m_pStateController);
		static_cast<CState_Silvermane*>(pair.second)->Set_Silvermane(this);
		static_cast<CState_Silvermane*>(pair.second)->Set_Transform(m_pTransform);
		static_cast<CState_Silvermane*>(pair.second)->Set_Model(m_pModel);
		static_cast<CState_Silvermane*>(pair.second)->Set_AnimationController(m_pAnimationController);
	}
	m_pStateController->Change_State(L"Idle");
	return S_OK;
}

CTransform* CSilvermane::Get_Transform() const
{
	return m_pTransform;
}

CModel* CSilvermane::Get_Model() const
{
	return m_pModel;
}

const _float CSilvermane::Get_PlusAngle() const
{
	return m_fPlusAngle;
}

const _float3& CSilvermane::Get_Dir() const
{
	return m_vDir;
}

void CSilvermane::Set_Move(const _bool _isMove)
{
	m_isMove = _isMove;
}

void CSilvermane::Set_EquipWeapon(const _bool _isEquipWeapon)
{
	m_isEquipWeapon = _isEquipWeapon;
	m_pWeapon->Set_Equip(m_isEquipWeapon);
}

void CSilvermane::Set_WeaponFixedBone(CHierarchyNode* _pFixedBone)
{
	if (m_pWeapon)
		m_pWeapon->Set_FixedBone(_pFixedBone);
}

void CSilvermane::Set_Camera(CCamera_Silvermane* _pCamera)
{
	m_pCamera = _pCamera;
}

const _bool CSilvermane::Is_EquipWeapon() const
{
	return m_isEquipWeapon;
}

void CSilvermane::Add_PlusAngle(const _float _fDeltaAngle)
{
	m_fPlusAngle += _fDeltaAngle * 400.f;
	if (360.f < m_fPlusAngle || -360.f > m_fPlusAngle)
		m_fPlusAngle = fmodf(m_fPlusAngle, 360.f);
	//if (-45.f > m_fPlusAngle)
	//	m_fPlusAngle = -45.f;
	//else if (45.f < m_fPlusAngle)
	//	m_fPlusAngle = 45.f;
}

void CSilvermane::Add_Dir(const _float _fX, const _float _fZ)
{
	_float fPower = 4.f;
	m_vDir.x += _fX * fPower;
	m_vDir.z += _fZ * fPower;

	if (-1.f > m_vDir.x) m_vDir.x = -1.f;
	if (1.f < m_vDir.x) m_vDir.x = 1.f;
	if (-1.f > m_vDir.z) m_vDir.z = -1.f;
	if (1.f < m_vDir.z) m_vDir.z = 1.f;
}

_int CSilvermane::Trace_CameraLook(const _double& _dDeltaTime)
{
	_vector svCameraLook = m_pCamera->Get_Look();
	_vector svLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	_vector svUp = m_pTransform->Get_State(CTransform::STATE_UP);

	svCameraLook = XMVector3Normalize(XMVectorSetY(svCameraLook, 0.f));
	svLook = XMVector3Normalize(XMVectorSetY(svLook, 0.f));
	_vector svAngle = XMVector3AngleBetweenVectors(svCameraLook, svLook);

	_float fRadian;
	XMStoreFloat(&fRadian, svAngle);

	svUp = XMVector3Normalize(svUp);
	_vector svCross = XMVector3Cross(svLook, svCameraLook);
	if (0.f < XMVectorGetY(svCross)) // 카메라가 왼쪽
	{
		m_pTransform->Rotation_Axis(svUp, (fRadian) * _dDeltaTime * 5.f);
		m_fAngle = XMConvertToDegrees(fRadian);
	}
	else if (0.f > XMVectorGetY(svCross)) // 카메라가 오른쪽
	{
		m_pTransform->Rotation_Axis(svUp, (fRadian) * -_dDeltaTime * 5.f);
		m_fAngle = -XMConvertToDegrees(fRadian);
	}
	else
		m_fAngle = 0.f;

#pragma region 실패 코드들
	// 추가 방향
	//if (m_isMove)
	//{
		//_vector svDirZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		//_vector svDirLook = svLook;

		//_vector svLookZAngle = XMVector3AngleBetweenVectors(svDirZ, svDirLook);
		//_float fLookZRadian; XMStoreFloat(&fLookZRadian, svLookZAngle);
		//_matrix smatLookZRotation = XMMatrixIdentity();

		//_vector svLookZCross = XMVector3Cross(svDirLook, svDirZ);
		//if (0.f < XMVectorGetY(svLookZCross)) // 룩벡터가 왼족
		//{
		//	smatLookZRotation = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fLookZRadian);
		//}
		//else if (0.f > XMVectorGetY(svLookZCross)) // 룩벡터가 오른쪽
		//{
		//	smatLookZRotation = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fLookZRadian);
		//}


		//_vector svDir = XMVector3Normalize(XMLoadFloat3(&m_vDir));
		////svDir = XMVector3TransformNormal(svDir, smatLookZRotation);
		//_vector svDirAngle = XMVector3AngleBetweenVectors(svDir, svDirZ);
		//_vector svDirCross = XMVector3Cross(svDir, svDirZ);
		//if (0.f < XMVectorGetY(svDirCross)) // 방향벡터가 왼쪽
		//{
		//	//m_pTransform->Rotation_Axis(svUp, -fDirRadian);
		//	//m_fDirAngle = -XMConvertToDegrees(fDirRadian);
		//	svDirAngle *= -1.f;
		//}
		//else if (0.f > XMVectorGetY(svDirCross)) // 방향벡터가 오른쪽
		//{
		//	//m_pTransform->Rotation_Axis(svUp, fDirRadian);
		//	//m_fDirAngle = XMConvertToDegrees(fDirRadian);
		//}
		//else
		//	m_fDirAngle = 0.f;

		//_float fDirRadian; XMStoreFloat(&fDirRadian, svDirAngle);
		//_matrix smatRotDir = XMMatrixRotationY(fDirRadian);
		//svLook = XMVector3TransformNormal(svLook, smatRotDir);
		//_vector svRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), svLook);
		//svUp = XMVector3Cross(svLook, svRight);
		//m_pTransform->Set_State(CTransform::STATE_LOOK, svLook);
		//m_pTransform->Set_State(CTransform::STATE_UP, svUp);
		//m_pTransform->Set_State(CTransform::STATE_RIGHT, svRight);
		//m_fDirAngle = XMConvertToDegrees(fDirRadian);
	//}

	//_vector svDirZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	//_vector svDir = XMVector3Normalize(XMLoadFloat3(&m_vDir));
	//_vector svDirAngle = XMVector3AngleBetweenVectors(svDir, svDirZ);
	//_float fDirRadian; XMStoreFloat(&fDirRadian, svDirAngle);
	//_vector svDirCross = XMVector3Cross(svDir, svDirZ);
	//if (0.f < XMVectorGetY(svDirCross)) // 방향벡터가 왼쪽
	//{
	//	//m_pTransform->Rotation_Axis(svUp, -fDirRadian);
	//	//m_fDirAngle = -XMConvertToDegrees(fDirRadian);
	//	fDirRadian *= -1.f;
	//}
	//else if (0.f > XMVectorGetY(svDirCross)) // 방향벡터가 오른쪽
	//{
	//	//m_pTransform->Rotation_Axis(svUp, fDirRadian);
	//	//m_fDirAngle = XMConvertToDegrees(fDirRadian);
	//}
	//else
	//	m_fDirAngle = 0.f;

	//_matrix smatPlusPivot = XMMatrixRotationAxis(svUp, fDirRadian);
	//m_pTransform->Set_PivotMatrix(smatPlusPivot);
	//m_fDirAngle = XMConvertToDegrees(fDirRadian);
#pragma endregion

	// 추가로 달리기와 같이 더 회전이 필요한 동작들을 회전시켜주는 부분입네다.
	_float fPlusRadian = XMConvertToRadians(m_fPlusAngle);

	_matrix smatPlusPivot = XMMatrixRotationAxis(svUp, fPlusRadian);
	_vector svRotCameraLook = XMVector4Transform(svCameraLook, smatPlusPivot);
	_vector svRotCameraRight = XMVector3Cross(svUp, svRotCameraLook);
	_vector svRotCameraUp = XMVector3Cross(svRotCameraLook, svRotCameraRight);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, svRotCameraRight);
	m_pTransform->Set_State(CTransform::STATE_UP, svRotCameraUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, svRotCameraLook);

 	return _int();
}

CSilvermane* CSilvermane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CSilvermane* pInstance = new CSilvermane(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSilvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSilvermane::Clone(void* _pArg)
{
	CSilvermane* pInstance = new CSilvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSilvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSilvermane::Free()
{
	Safe_Release(m_pStateController);
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}

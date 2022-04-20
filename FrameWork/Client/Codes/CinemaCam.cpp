#include "pch.h"
#include "CinemaCam.h"
#include "HierarchyNode.h"
#include "Animation.h"

CCinemaCam::CCinemaCam()
	:m_pCamera(nullptr)
	, m_pModel(nullptr)
	, m_iShortTag(0)
{
}

CCinemaCam::CCinemaCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pCamera(nullptr)
	, m_pModel(nullptr)
	, m_iShortTag(0)
{
}

CCinemaCam::CCinemaCam(const CCinemaCam& rhs)
	: CGameObject(rhs)
	, m_pCamera(rhs.m_pCamera)
	, m_pModel(rhs.m_pModel)
	, m_iShortTag(0)
{
	Safe_AddRef(m_pCamera);
	Safe_AddRef(m_pModel);
}

HRESULT CCinemaCam::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;
		

	return S_OK;
}

HRESULT CCinemaCam::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	//CCamera::CAMERADESC tDesc = (*(CCamera::CAMERADESC*)pArg);
	CINEMADESC tDesc = (*(CINEMADESC*)pArg);

	m_pCamTag = tDesc.tCameraDesc.pCameraTag;
	
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"CameraCom", (CComponent**)&m_pCamera, &tDesc.tCameraDesc)))
		return E_FAIL;

	if(FAILED(Set_Camera(tDesc.iShotTag)))
		return E_FAIL;

	m_pModel->SetUp_AnimationIndex(0);

	return S_OK;
}

_int CCinemaCam::Tick(_double TimeDelta)
{
	//BornÀÌ¸§ : Camera_01_born
	m_pModel->Update_CombinedTransformationMatrix(TimeDelta);

	/*_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(3.f, -5.f, 15.f);
	m_pModel->Set_PivotMatrix(matPivot);*/
	CHierarchyNode* pBorn = Get_CamBone();
	_matrix matBone = pBorn->Get_CombinedMatrix()*m_pModel->Get_PivotMatrix();
	//matBone *= (XMMatrixRotationY(XMConvertToRadians(270.f))*XMMatrixTranslation(3.f,-5.f,15.f));
	matBone *= m_pTransform->Get_WorldMatrix();
	m_pCamera->Update_Matrix(matBone);

	Get_CamMoveEnd();

	return _int();
}

_int CCinemaCam::LateTick(_double TimeDelta)
{
	return _int();
}

_bool CCinemaCam::Get_CamMoveEnd()
{
	if (!m_pModel)
		return false;
	vector<CAnimation*> vecAnim = m_pModel->Get_Animations();
	cout << vecAnim[0]->Get_TrackPoition() << endl;
	if (vecAnim[0]->Is_Finished())
		return true;

	return _bool();
}

_bool CCinemaCam::Get_Event(_double dFrame)
{
	vector<CAnimation*> vecAnim = m_pModel->Get_Animations();
	if (vecAnim[0]->Get_TrackPoition() >= dFrame)
		return true;

	return false;
}

_double CCinemaCam::Get_CamFrame()
{
	vector<CAnimation*> vecAnim = m_pModel->Get_Animations();

	return vecAnim[0]->Get_TrackPoition();
}

void CCinemaCam::Set_CameraMatrix(_fmatrix matPivot)
{
	_matrix matTransform = XMMatrixIdentity();
	matTransform *= matPivot;
	m_pTransform->Set_WorldMatrix(matTransform);
}

void CCinemaCam::Reset_Camera()
{
	vector<CAnimation*> vecAnim = m_pModel->Get_Animations();
	vecAnim[0]->Reset_Animation();
}

void CCinemaCam::Add_Fov(_float fSpeed)
{
	m_pCamera->Change_Fov(fSpeed);
}

void CCinemaCam::Minus_Fov(_float fSpeed)
{
	m_pCamera->Change_Fov(-fSpeed);
}

HRESULT CCinemaCam::Set_Camera(_uint iSceneTag)
{
	HRESULT hr = S_OK;
	switch (iSceneTag)
	{
	case (_uint)CINEMA_INDEX::CINEMA1_1:
		hr = CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Cam1_1", L"CamModel", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_INDEX::CINEMA1_2:
		hr = CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Cam1_2", L"CamModel", (CComponent**)&m_pModel);
		break;
	case (_uint)CINEMA_INDEX::CINEMA2_1:
		hr = CGameObject::SetUp_Components(m_iSceneID, L"Model_Cinema_Cam2_1", L"CamModel", (CComponent**)&m_pModel);
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	m_iShortTag = iSceneTag;

	return S_OK;
}

CHierarchyNode* CCinemaCam::Get_CamBone()
{
	CHierarchyNode* pBorn = nullptr;
	switch (m_iShortTag)
	{
	case (_uint)CINEMA_INDEX::CINEMA1_1:
		pBorn=m_pModel->Get_BoneMatrix("Camera_01_born");
		break;
	case (_uint)CINEMA_INDEX::CINEMA1_2:
		pBorn=m_pModel->Get_BoneMatrix("Camera_02_born");
		break;
	case (_uint)CINEMA_INDEX::CINEMA2_1:
		pBorn=m_pModel->Get_BoneMatrix("Shot_003_01_born");
		break;
	}
	return pBorn;
}

void CCinemaCam::Change_CurrentCam()
{
	g_pGameInstance->Change_BaseCamera(m_pCamTag);
}

CCinemaCam* CCinemaCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCinemaCam* pInstance = new CCinemaCam(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCinemaCam Create Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCinemaCam::Clone(const _uint iSceneID, void* pArg)
{
	CCinemaCam* pInstance = new CCinemaCam(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CCinemaCam Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCinemaCam::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCamera);
	Safe_Release(m_pModel);
}

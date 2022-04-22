#pragma once
#ifndef __CINEMACAM_H__
#define __CINEMACAM_H__
#include "GameObject.h"

BEGIN(Client)
class CCinemaCam final : public CGameObject
{
public:
	typedef struct tagCinema
	{
		CCamera::CAMERADESC tCameraDesc;
		_uint iShotTag;
	}CINEMADESC;
private:
	explicit CCinemaCam();
	explicit CCinemaCam(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCinemaCam(const CCinemaCam& rhs);
	virtual ~CCinemaCam() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
public:
	_bool Get_CamMoveEnd();
	_bool Get_Event(_double dFrame);
	_double Get_CamFrame();
	void Set_CameraMatrix(_fmatrix matPivot);
	void Reset_Camera();
	void Add_Fov(_float fSpeed);
	void Minus_Fov(_float fSpeed);
	void Set_Fov(_float fSpeed, _float fGoalAngle);
	void Set_Fov(_float fAngle);
private:
	HRESULT Set_Camera(_uint iSceneTag);
	class CHierarchyNode* Get_CamBone();
public:
	void Change_CurrentCam();
public:
	static CCinemaCam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	CCamera* m_pCamera;
	wstring m_pCamTag;
	CModel* m_pModel;
	_uint m_iShortTag;
};
END
#endif
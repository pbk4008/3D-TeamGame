#pragma once
#ifndef __CINEMACAM_H__
#define __CINEMACAM_H__
#include "GameObject.h"
#include "Camera.h"

BEGIN(Client)
class CCinemaCam final : public CGameObject
{
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
	void Change_CurrentCam();
public:
	static CCinemaCam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	CCamera* m_pCamera;
	wstring m_pCamTag;
};
END
#endif
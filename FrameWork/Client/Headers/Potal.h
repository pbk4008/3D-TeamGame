#pragma once
#ifndef __POTAL_H__
#define __POTAL_H__

#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CMeteor;
class CPotal final : public CGameObject
{
private:
	explicit CPotal();
	explicit CPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPotal(const CPotal& rhs);
	virtual ~CPotal() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Component();
private:
	_uint Scaling(_double dDeltaTime);
	_uint Create_Meteor();
	_uint Remove_Portal(_double dDeltaTime);
public:
	static CPotal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContex);
	virtual CGameObject* Clone(_uint iSeneid, void* pArg);
private:
	virtual void Free() override;
private:
	CVIBuffer_Rect* m_pRect;
	CTexture* m_pDiffuseTexture;
	CTexture* m_pMaskTexture;
private:/*For Create Meteor*/
	_bool m_bCreate;
	CMeteor* m_pMeteor;
};
END
#endif
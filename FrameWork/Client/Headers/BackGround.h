#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "GameObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CBackGround final : public CGameObject
{
private:
	explicit CBackGround();
	explicit CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double fDeltaTime) override;
	virtual _int LateTick(_double fDeltaTime) override;
	virtual HRESULT Render() override;
public:
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	HRESULT Ready_GameObject(void* pArg);
private:
	virtual void Free() override;
private:
	CVIBuffer_Rect* m_pRcTex;
	CTexture* m_pTexture;

private:
	_float2 m_fPos;
	_float2 m_fSize;
};
END
#endif
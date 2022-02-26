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
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double fDeltaTime) override;
	virtual _int LateTick(_double fDeltaTime) override;
	virtual HRESULT Render() override;
public:
	virtual CGameObject* Clone(void* pArg) override;
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	HRESULT Ready_GameObject(void* pArg);
private:
	virtual void Free() override;
private:
	CVIBuffer_Rect* m_pRcTex;
	CTexture* m_pTexture;
};
END
#endif
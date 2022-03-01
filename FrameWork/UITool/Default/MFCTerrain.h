#pragma once

#ifndef MFCTerrain_h__
#define MFCTerrain_h__

#include "UI_Tool_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Terrain;
END

class CMFCTerrain final : public CGameObject
{
protected:
	explicit CMFCTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCTerrain(const CGameObject& rhs);
	virtual ~CMFCTerrain() = default;

public:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype() override;
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetUp_Shader();

private:
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;

private:
	virtual HRESULT SetUp_Components();

public:
	static CMFCTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

#endif // MFCTerrain_h__



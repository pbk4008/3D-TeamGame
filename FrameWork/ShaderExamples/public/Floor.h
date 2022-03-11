#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Example)
class CFloor final : public CGameObject
{
protected:
	explicit CFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CFloor(const CFloor& rhs);
	virtual ~CFloor() = default;
public:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype() override;
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
	virtual HRESULT	Render_ShadeShadow(ID3D11ShaderResourceView* ShadowMap) override;
	virtual HRESULT	Render_PBR() override;
private:
	CTexture*	m_pTexture = nullptr;
	CTexture*	m_pFilterTexture = nullptr;
	CTexture*	m_pBrushTexture = nullptr;

	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;
	const LIGHTDESC*	m_LightDesc = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CFloor* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

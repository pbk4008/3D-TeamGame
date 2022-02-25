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
	explicit CFloor(const CGameObject& rhs);
	virtual ~CFloor() = default;
public:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype() override;
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CFloor* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

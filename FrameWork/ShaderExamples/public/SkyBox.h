#pragma once

#include "GameObject.h"
#include "Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Example)
class CSkyBox final : public CGameObject
{
private:
	explicit CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSkyBox(const CGameObject& rhs);
	virtual ~CSkyBox() = default;
public:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype() override;
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CSkyBox* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

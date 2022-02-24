#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCamera;
class CTransform;
END


BEGIN(Example)
class CDynamicCaemra final : public CGameObject
{
private:
	explicit CDynamicCaemra(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDynamicCaemra(const CDynamicCaemra& rhs);
	virtual ~CDynamicCaemra() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
public:
	static CDynamicCaemra* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private: virtual HRESULT SetUp_Components(); 

private: CCamera*		m_pCamCom = nullptr;
private: CTransform*	m_pTransformCom = nullptr;
};
END


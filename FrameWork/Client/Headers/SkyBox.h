#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:
	explicit CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSkyBox(const CGameObject& rhs);
	virtual ~CSkyBox() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*		m_pTexture = nullptr;
	CTexture*		m_pTexture2 = nullptr;
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CSkyBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	virtual void Free() override;
};

END
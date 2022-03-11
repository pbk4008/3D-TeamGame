#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CModel;
class CSwordTrail;
class CTexture;
class CHierarchyNode;
END

BEGIN(Example)
class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	/* 원형객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct_Prototype() override;
	/* 복제본객체가 생성될때 호출되는 함수. */
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow()override;
	virtual HRESULT	Render_ShadeShadow(ID3D11ShaderResourceView * ShadowMap) override;
	virtual HRESULT	Render_PBR() override;

private:
	CModel*					m_pModelCom = nullptr;
	vector<CTexture*>		m_vecTextureCom;;

	const LIGHTDESC*		m_Lightdesc = nullptr;
	_bool					m_bNomralMapping = false;

	// Trail
	_float					m_fTimer = 0;
	_float					m_fLifeTime = 1.f;
	_matrix					m_hadeMatrix = XMMatrixIdentity();
	CGameObject*			m_pSwordTrail;
	_bool					m_bTrailOnOff = false;

private: CHierarchyNode* m_pWeponNode = nullptr;

private: virtual HRESULT SetUp_Components();
private: HRESULT SetUp_Weapon();

public: static CPlayer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
public: virtual CGameObject* Clone(void* pArg) override;
public: virtual void Free() override;
};
END


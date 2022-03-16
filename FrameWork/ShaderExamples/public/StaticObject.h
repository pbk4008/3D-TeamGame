#pragma once

#include "GameObject.h"
#include "Defines.h"
#include "DataLoader.h"

BEGIN(Engine)
class CModel;;
END

BEGIN(Example)
class CStaticObject final : public CGameObject
{
private: explicit CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext);
private: explicit CStaticObject(const CStaticObject& rhs);
private: virtual ~CStaticObject() = default;

public: void				Set_WVPMatrix();
public: void				Set_LightMatrix();

public: virtual HRESULT		NativeConstruct_Prototype()							override;
public: virtual HRESULT		NativeConstruct(void* pArg)							override;
public: virtual _int		Tick(_double TimeDelta)								override;
public: virtual _int		LateTick(_double TimeDelta)							override;
public: virtual HRESULT		Render()											override;
public: virtual HRESULT		Render_Shadow()										override;
public: virtual HRESULT		Render_ShadeShadow(ID3D11ShaderResourceView* SRV)	override;
public: virtual HRESULT		Render_PBR()										override;

private: virtual HRESULT	SetUp_Components();

private: CModel*				m_pModel = nullptr;
private: MODELDESC				m_ModelDesc;
private: MESHDESC				m_MeshDesc;
private: const LIGHTDESC*		m_Lightdesc = nullptr;
private: _bool					m_bNomralMapping = false;

public: static CStaticObject*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
public: virtual CStaticObject*	Clone(void* pArg) override;
public: virtual void			Free(void) override;

};
END
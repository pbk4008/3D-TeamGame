#pragma once
#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "Base.h"
BEGIN(Engine)
class CTexture;
class CMaterial final : public CBase 
{
private:
	explicit CMaterial();
	explicit CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMaterial(const CMaterial& rhs);
	virtual~CMaterial() = default;
public:
	HRESULT Native_Construct_ProtoType();
	HRESULT Render();
public:
	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CMaterial* Clone();
private:
	virtual void Free() override;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	_tchar		m_pMeshTextureName[AI_TEXTURE_TYPE_MAX][MAX_PATH];
	vector<CTexture*> m_pMeshTexture;
private:
	ID3DX11Effect* m_pEffect;
	vector<EFFECTDESC*>	m_PassDesc;
private:
	_bool m_isCloned;
};
END
#endif
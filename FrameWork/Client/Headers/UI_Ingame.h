#pragma once

#ifndef UI_Ingame_h__
#define UI_Ingame_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
class CTransform;
class CMouse;
END

BEGIN(Client)
class CUI_Ingame final : public CUI
{
public:
	typedef struct tagUIDesc
	{
		 _tchar TextureTag[MAX_PATH];
		_float3 fPos;
		_float2 fScale;
	}UIDESC;

private:
	explicit CUI_Ingame(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Ingame(const CUI& rhs);
	virtual ~CUI_Ingame() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIDesc(UIDESC Desc) { m_Desc = Desc; }
	UIDESC Get_UIDesc() { return m_Desc; }

private:
	UIDESC m_Desc;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Ingame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Ingame_h__

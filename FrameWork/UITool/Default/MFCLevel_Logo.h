#pragma once

#ifndef MFCLevel_Logo_h__
#define MFCLevel_Logo_h__

#include "UI_Tool_Define.h"
#include "Level.h"

BEGIN(Engine)
class CMouse;
END
class CMFCLevel_Logo final : public CLevel
{
public:
	explicit CMFCLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMFCLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Open_Level();

private:
	class CMouse* m_pMouse = nullptr;
	class CMFCObject_UI* m_pMFCUI = nullptr;

public:
	static CMFCLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

#endif // MFCLevel_Logo_h__



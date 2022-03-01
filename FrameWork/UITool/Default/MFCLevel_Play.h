#pragma once

#ifndef MFCLevel_Play_h__
#define MFCLevel_Play_h__

#include "UI_Tool_Define.h"
#include "Level.h"

BEGIN(Engine)
END
class CMFCLevel_Play final : public CLevel
{
public:
	explicit CMFCLevel_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMFCLevel_Play() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Open_Level();

private:
	class CMFCEffect* m_pMFCEffect = nullptr;

public:
	static CMFCLevel_Play* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

#endif // MFCLevel_Play_h__



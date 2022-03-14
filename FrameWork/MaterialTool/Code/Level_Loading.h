#pragma once

#include "Level.h"

BEGIN(Tool)

class CLoader;

class CLevel_Loading final : public CLevel
{
private:
	explicit CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT NativeConstruct(const ELevel _eNextLevel);
	virtual _int Tick(_double _dDeltaTime);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_BackGround(const wstring& _wstrLayerTag);
	HRESULT Ready_Layer_UI(const wstring& _wstrLayerTag);

private:
	HRESULT Open_Level();

private:
	ELevel m_eNextLevel = ELevel::Max;
	CLoader* m_pLoader = nullptr;

public:
	static CLevel_Loading* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext, const ELevel _eNextLevel);
	virtual void Free() override;
};

END
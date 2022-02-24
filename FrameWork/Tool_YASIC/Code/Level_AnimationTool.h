#pragma once

#include "Level.h"

BEGIN(Tool)

class CLevel_AnimationTool final : public CLevel
{
private:
	explicit CLevel_AnimationTool(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext);
	virtual ~CLevel_AnimationTool() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double _dDeltaTime);
	virtual HRESULT Render();

public:
	static CLevel_AnimationTool* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeivceContext);
	virtual void Free() override;
};

END
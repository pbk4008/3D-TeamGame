#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_Skill_Meter : public CUI
{
private:
	explicit CUI_Skill_Meter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CUI_Skill_Meter(const CUI_Skill_Meter& _rhs);
	virtual ~CUI_Skill_Meter() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public:
	static CUI_Skill_Meter* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
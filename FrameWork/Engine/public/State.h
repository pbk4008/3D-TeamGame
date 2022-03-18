#pragma once

#include "Base.h"

BEGIN(Engine)

class CStateController;

class ENGINE_DLL CState abstract : public CBase
{
protected:
	explicit CState(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CState(const CState& _rhs);
	virtual ~CState() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState();
	virtual HRESULT ExitState(void* pArg);

public:
	const wstring& Get_Tag() const;

	void Set_StateController(CStateController* _pStateController);

protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	CStateController* m_pStateController = nullptr;
	wstring m_wstrTag = L"";

public:
	virtual void Free() override;
};

END
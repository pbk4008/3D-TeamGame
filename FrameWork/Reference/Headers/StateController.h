#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CState;

class ENGINE_DLL CStateController final : public CComponent
{
public:
	enum class EChange { Normal, NonEnter, NonExit, NonBoth, Max };
private:
	explicit CStateController(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CStateController(const CStateController& _rhs);
	virtual ~CStateController() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	_int Tick(const _double & _dDeltaTime);
	_int LateTick(const _double & _dDeltaTime);
	HRESULT Render();

public:
	CGameObject* Get_GameObject() const;
	unordered_map<wstring, CState*>& Get_States();
	
	void Set_GameObject(CGameObject* _pGameObject);

public:
	HRESULT Add_State(const wstring & _wstrStateTag, CState * _pState);
	HRESULT Change_State(const wstring& _wstrStateTag, const EChange _eChange = EChange::Normal);

private:
	CGameObject* m_pGameObject = nullptr;
	unordered_map<wstring, CState*> m_mapStates;

	_bool m_isChange = false;

	wstring m_wstrPreStateTag = L"";
	wstring m_wstrCurStateTag = L"";

	CState* m_pPreState = nullptr;
	CState* m_pCurState = nullptr;

public:
	static CStateController* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext);
	virtual CComponent* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
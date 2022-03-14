#pragma once

#ifndef UI_Monster_Panel_h__
#define UI_Monster_Panel_h__

#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Trapezium;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_Monster_Panel : public CUI
{
public:
	typedef struct tagUIDesc
	{
		CTransform* pTargetTransform;
	}PANELDESC;

protected:
	explicit CUI_Monster_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Monster_Panel(const CUI& rhs);
	virtual ~CUI_Monster_Panel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_TargetWorldMatrix(_matrix matTarget) { XMStoreFloat4x4(&m_TargetMatrix, matTarget);}

private:
	PANELDESC m_Desc;
	_float4x4 m_TargetMatrix;

	class CUI_Monster_Back* m_pUIBack = nullptr;
	class CUI_Monster_Level* m_pUILevel = nullptr;
	class CUI_Monster_HpBar* m_pUIHpBar = nullptr;
private:
	virtual HRESULT SetUp_Components();

public:
	static CUI_Monster_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#endif // UI_Monster_Panel_h__

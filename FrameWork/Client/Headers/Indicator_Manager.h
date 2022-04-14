#pragma once
#ifndef __Indicator_Manager_H__
#define __Indicator_Manager_H__

#include "UI_Indicator.h"
#include "Object_Manager.h"
#include "SingleTon.h"

BEGIN(Client)
class CIndicator_Manager final : public CSingleTon<CIndicator_Manager>
{
	friend CSingleTon;
private:
	NO_COPY(CIndicator_Manager);
	explicit CIndicator_Manager();
	virtual ~CIndicator_Manager()=default;
public:
	HRESULT Ready_Indicator_Manager(ID3D11Device * pDevcie, ID3D11DeviceContext * pDeviceContext);
	HRESULT Add_Indicator(_uint iSceneID, const wstring & pLayerTag, _uint iCount);

	void Active_Indicator();
	void Setting_TargetPos();
	
	CUI_Indicator* Get_Indicator(_uint iIndicatorIndex);

private:
	virtual void Free() override;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	
private:
	vector<CUI_Indicator*> m_vecIndicator;
	_uint m_iHitNum = 0;
};
END

#endif
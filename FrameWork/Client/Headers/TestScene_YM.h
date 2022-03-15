#ifndef TestScene_YM_h__
#define TestScene_YM_h__

#include "Level.h"

class CTestScene_YM final : public CLevel
{
private:
	explicit CTestScene_YM();
	explicit CTestScene_YM(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CTestScene_YM() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Gameobject();

public:
	static CTestScene_YM* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);

private:
	virtual void Free() override;
};

#endif // TestScene_YM_h__

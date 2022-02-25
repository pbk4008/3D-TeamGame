#ifndef Plane_h__
#define Plane_h__

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CRenderer;
END

class CPlane final : public CGameObject
{
protected:
	explicit CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CPlane(const CPlane& _rhs);
	virtual ~CPlane(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta)override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT SetUp_Components();

public:
	void			Set_WVPMatrix();

public:
	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;

public:
	static CPlane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free(void) override;
};


#endif // Plane_h__
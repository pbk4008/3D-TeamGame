#ifndef Dyanmic_Camera_h__
#define Dyanmic_Camera_h__

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CRenderer;
class CCamera;
END

class CDynamic_Camera final : public CGameObject
{
protected:
	explicit CDynamic_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CDynamic_Camera(const CDynamic_Camera& _rhs);
	virtual ~CDynamic_Camera(void) = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	LateTick(_double TimeDelta) override;
	virtual HRESULT Render()					override;
public:
	virtual HRESULT SetUp_Components();
public:
	void			Input_Key(_double _dTimeDelta);
public:
	CCamera*		 m_pCamCom = nullptr;
public:
	static CDynamic_Camera* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject*	Clone(void* _pArg) override;
	virtual void		    Free(void)		   override;
};

#endif // Dyanmic_Camera_h__

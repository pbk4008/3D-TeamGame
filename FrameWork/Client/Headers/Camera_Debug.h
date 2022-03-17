#ifndef Dyanmic_Camera_h__
#define Dyanmic_Camera_h__

#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CRenderer;
class CCamera;
END

BEGIN(Client)

class CCamera_Debug final : public CGameObject
{
protected:
	explicit CCamera_Debug(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CCamera_Debug(const CCamera_Debug& _rhs);
	virtual ~CCamera_Debug(void) = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	LateTick(_double TimeDelta) override;
	virtual HRESULT Render()					override;
public:
	virtual HRESULT SetUp_Components();
public:
	void			Input_Key(_double _dTimeDelta);
public:
	CCamera*		 m_pCamera = nullptr;
public:
	static CCamera_Debug* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject*	Clone(const _uint _iSceneID, void* _pArg) override;
	virtual void		    Free(void)		   override;
};

END

#endif // Dyanmic_Camera_h__
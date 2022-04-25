#ifndef Loot_Shield_h__
#define Loot_Shield_h__

#include "InteractableObject.h"
#include "ItemData.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CInventoryData;
class CLoot_Shield final : public CInteractableObject
{
public:
	struct Desc
	{

	};

public:
	explicit CLoot_Shield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CLoot_Shield(const CLoot_Shield& _rhs);
	virtual ~CLoot_Shield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

public:
	virtual _bool   IsInteractable(void)    override;
	virtual _bool   HasInteractionUI(void)  override;
	virtual _float  GetRatioForUI(void)		override;
	virtual _vector GetGuideUIPos(void)		override;

public:
	virtual void BeginInteraction(void) override;
	virtual void UpdateInteraction(_double _dTimeDelta) override;
	virtual void EndInteraction(void) override;

public:
	void  Take(void); /* hide */
	void  SetTakableState(_bool bState);
	_bool IsTakable(void);
	
private:
	CGameObject*	 m_pPlayer = nullptr;

private:
	_float		 m_interactDist = 1.0f;
	_bool		 m_bTakable = true;
	_bool		 m_bLateTick = false;

private:
	CInventoryData* m_pInventoryData = nullptr;
	
public:
	static CLoot_Shield* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Loot_Shield_h__

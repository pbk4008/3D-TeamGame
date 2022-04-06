#ifndef DropObject_h__
#define DropObject_h__

#include "InteractableObject.h"
#include "ItemData.h"

BEGIN(Engine)
class CSplineCurve;
class CModel;
END

BEGIN(Client)
class CInventoryData;
class CDropObject final : public CInteractableObject
{
public:
	struct Desc
	{

	};

public:
	explicit CDropObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CDropObject(const CDropObject& _rhs);
	virtual ~CDropObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components();

public:
	virtual _bool   IsInteractable(void) override;
	virtual _bool   HasInteractionUI(void) override;
	virtual _float  GetRatioForUI(void) override;
	virtual _vector GetGuideUIPos(void) override;

public:
	virtual void BeginInteraction(void) override;
	virtual void UpdateInteraction(_double _dTimeDelta) override;
	virtual void EndInteraction(void) override;

public:
	void  Drop(CItemData itemData, _fvector dropPos, EScatterType scatterType = EScatterType::Quad, CTransform* pSender = nullptr); // show
	void  Take(void); /* hide */
	void  SetTakableState(_bool bState);
	_bool IsDrop(void);
	_bool IsTakable(void);
	
private:
	CGameObject*	 m_pPlayer = nullptr;
	CModel*			 m_pModel = nullptr;

private:
	CItemData	 m_droppedItem;
	_float3      m_dropPos;
	_float3		 m_scatteredPos;
	_float		 m_interactDist = 1.0f;
	_float		 m_elapsed = 0.0f;
	_bool		 m_bDrop = false;
	_bool		 m_bTakable = false;
	_bool		 m_bLateTick = false;
	const _float m_scatteredRadius = 3.5f;
	const _float m_dropDurtaion = 1.0f;

private:
	CSplineCurve*   m_pSplineCurve = nullptr;
	CInventoryData* m_pInventoryData = nullptr;
	
public:
	static CDropObject* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
#endif // DropObject_h__

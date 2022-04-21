#pragma once
#ifndef __CINEMA_ACTOR_H__
#define __CINEMA_ACTOR_H__

BEGIN(Client)
class CCinemaActor final :public CActor
{
private:
	explicit CCinemaActor();
	explicit CCinemaActor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCinemaActor(const CCinemaActor& rhs);
	virtual ~CCinemaActor() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Ready_Model(_uint iModel);
	HRESULT Render_Acoter();
public:
	void Actor_AnimPlay(_int iIndex);
	void AnimSpeed(_float fSpeed);
	void Actor_AnimReset();
public:
	static CCinemaActor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);
private:
	virtual void Free()override;
private:
	CAnimationController* m_pController;
	_uint m_iActorTag;
};
END
#endif
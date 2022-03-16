#ifndef Monster_Bastion_2HSword_h__
#define Monster_Bastion_2HSword_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CWeapon;
class CMonster_Bastion_2HSword final : public CActor
{
public:
	enum class ANIM_TYPE { ANIM_HEAD, ANIM_IDLE, ANIM_ATTK, ANIM_END };

private:
	explicit CMonster_Bastion_2HSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_2HSword(const CMonster_Bastion_2HSword& _rhs);
	virtual ~CMonster_Bastion_2HSword() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	void	Check_DistanceForPlayer(void);
private:
	HRESULT Ready_Components();
	HRESULT Ready_Weapon();
	HRESULT Ready_AnimFSM(void);

private:
	CModel*				  m_pModel = nullptr;
	CAnimator*			  m_pAnimator = nullptr;

private:
	CAnimator::ANIMATORDESC m_AanimDesc;

private: /* For.Weapon */
	CWeapon* m_pCurWeapon = nullptr;
	_bool m_isEquipWeapon = false;
	unordered_map<wstring, CWeapon*> m_umapWeapons;

public:
	static CMonster_Bastion_2HSword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END


#endif // Monster_Bastion_2HSword_h__
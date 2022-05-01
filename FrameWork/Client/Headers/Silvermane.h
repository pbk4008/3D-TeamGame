#pragma once

#include "Actor.h"
#include "Weapon.h"
#include "InventoryData.h"
#include "PlayerData.h"

BEGIN(Engine)

END

BEGIN(Client)
#pragma region 전방선언
class CWeapon;
class CShield;
class CFlyingShield;
class CCamera_Silvermane;
class CJumpNode;
class CJumpTrigger;
class CJumpBox;
class CUI_Blank_CKey;
class CUI_Blank_FKey;
class CUI_Fill_Ckey;
class CUI_Fill_Ckey2;
class CInventoryData;
class CEquipmentData;
class CPlayerData;
class CDropBox;
#pragma endregion


class CSilvermane final : public CActor
{
public:
	typedef struct tagSceneMove
	{
		_uint iMaxHp;
		_uint iCurHp;
		_float3 vPos;
	}SCENEMOVEDATA;
private:
	explicit CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane(const CSilvermane& _rhs);
	virtual ~CSilvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow()  override;
	virtual HRESULT	Render_Velocity()  override;
	HRESULT Render_Debug();
private:
	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Weapons(const _uint _iSceneID);

public:
	virtual void OnCollisionEnter(CCollision& collision) override;
	virtual void OnCollisionStay(CCollision& collision) override;
	virtual void OnCollisionExit(CCollision& collision) override;
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void OnTriggerExit(CCollision& collision) override;
	virtual void OnControllerColliderHit(CCollision& collision) override;
	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;
	virtual void Parry(const PARRYDESC& _tParryDesc) override;

public:
	const _bool IsHit() const;
	const _bool IsDash() const;
	const _bool IsGround() const;

	CTransform* Get_Transform() const;
	CModel* Get_Model() const;
	const _float Get_PlusAngle() const;
	const _float Get_Angle() const;
	//플레이어 씬 이동시 다음씬으로 넘어가야 할 데이터 생성 후 밖으로 빼내기
	const SCENEMOVEDATA Get_SceneMoveData() const;

	void  Set_Radial(_bool check);
	void  Set_RadialCnt(_int radialCnt);
	_bool Get_RadialCheck() { return m_pRenderer->Get_RenderButton(CRenderer::RADIAL); }

	void Set_IsHit(const _bool _isHit);
	void Set_IsFall(const _bool _isFall);
	void Set_IsDash(const _bool _isDash);
	void Set_IsMove(const _bool _isMove);
	void Set_IsTrasceCamera(const _bool _isTraceCamera);
	void Set_IsDead(const _bool _isDead);

	void Set_Camera(CCamera_Silvermane* _pCamera);
	void Set_PlusAngle(const _float _fAngle);
	void Set_Position(const _float3 _vPosition);
	virtual void Set_FootPosition(const _float3& _vPos) override;

	void Add_PlusAngle(const _float _fDeltaAngle);
	void Add_Velocity(const CTransform::STATE _eState, const _double& _dDeltaTime);
	void Add_HP(const _float _fValue);

	void Respawn();

public: /* For.Weapon */
	const _bool IsEquipWeapon() const;
	const CWeapon::EType Get_WeaponType() const;
	virtual void Set_IsAttack(const _bool _isAttack);
	void Set_IsTrail(const _bool _isTrail);
	void Set_EquipWeapon(const _bool _isEquipWeapon);
	void Set_WeaponFixedBone(const string& _wstrFixedBoneTag);
	void Set_WeaponFixedBone(CHierarchyNode* _pFixedBone);
	void Set_WeaponActive(const _bool _isActive);
	const _bool Change_Weapon(const wstring& _name);
	const _bool Change_Weapon(void);
	void Change_WeaponOnInventory(void);

	HRESULT			Change_State(const wstring& _wstrStateTag);
	void			RangeAttack(const _float _fForce = 1.f);
	CWeapon*		Get_CurerntWeapon() { return m_pCurWeapon; }
	CInventoryData* Get_InvenData(void) { return m_pInventoryData; }
	CPlayerData*	Get_PlayerData(void) { return m_pPlayerData; }

public: /* For.Shield */
	const _bool IsEquipShield() const;
	const _bool IsShieldThrow() const;
	const _bool IsShieldReturn() const;
	const _bool IsCoolTime_ShieldThrow() const;
	const _float Get_BlockTime() const;
	void Set_EquipShield(const _bool _isEquipShield);
	void Set_EquipShieldAnim(const _bool _isEquipShield);
	void Set_BlockTime(const _float _fValue);
	void Set_IsShieldAttack(const _bool _isAttack);
	void Set_IsShieldThrow(const _bool _isShieldThrow);
	void Set_IsShieldReturn(const _bool _isShieldReturn);
	void Set_IsShieldCoolTime(const _bool _isCoolTime);
	void Add_BlockTime(const _float _fValue);
	HRESULT ThrowShield(const _fvector& _svTargetPos);
	void Return_Shield();
	void End_ThrowShield();
	void Loot_Shield();
	_bool IsLootShield() { return m_isLootShield; }

public: /* For.JumpNode */
	CJumpNode* Get_TargetJumpNode() const;
	CJumpTrigger* Get_TargetJumpTrigger() const;
	CJumpBox* Get_TargetJumpBox() const;
	const _bool Raycast_JumpNode(const _double& _dDeltaTime);

public: /* For.DropBox */
	CDropBox* Get_TargetDropBox() const;
	void Set_IsBoxOpen(const _bool _isBoxOpen);
	const void Raycast_DropBox(const _double& _dDeltaTime);

public: /* For.Execute */
	void Set_Execution(const _bool _isExecution, CActor* _pTarget = nullptr);
	CActor* Get_TargetExecution() const;
	void	Set_TargetExecution(CActor* pTarget);
	CHierarchyNode* Get_ExecutionTargetBone() const;
	CUI_Blank_FKey* Get_Blank_FKey() const;

public: /* For.Skill */
	const _bool IsSkill() const;
	const _float Get_SkillGuage() const;
	void Set_IsSkill(const _bool _isSkill);
	void Set_Skill(const _bool _isSkil);
	void Add_SkillGuage(const _float _fValue);

public: void	Set_HealActive(_bool check) { m_bhealcheck = check; }
public: void	Set_LightColor(_fvector color) { m_lightcolor = color; }

private:
	const _int	Trace_CameraLook(const _double& _dDeltaTime);
	const _int	Input(const _double& _dDeltaTime);
	RIM			ColorChange_RimCheck(RIM& rimdesc);

public:  HRESULT	Create_MotionTrail(_int idex, _bool runcheck = false, _bool throwcheck = false, _float colorindx = 0.f);
//public:  _bool		

private: /* Components */
	CStateController* m_pStateController = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

public:
	CCamera_Silvermane* m_pCamera = nullptr;

	_bool m_isHit = false;
	_bool m_isDash = false;
	_bool m_isFall = false;
	_bool m_isMove = false;
	_bool m_isBlock = false;
	_bool m_isTraceCamera = true;

	_float m_fMoveSpeed = 0.f;
	_float m_fAngle = 0.f;
	_float m_fPlusAngle = 0.f;
	_float3 m_vRespawnPos = { 0.f, 0.f, 0.f };

private: /* For.Weapon */
	CWeapon* m_pCurWeapon = nullptr;
	CWeapon* m_pNeedle = nullptr;
	_bool m_isEquipWeapon = false;

private: /* For.Shield */
	CShield* m_pShield = nullptr;
	CFlyingShield* m_pFlyingShield = nullptr;
	_bool m_isEquipShield = false;
	_bool m_isShieldThrow = false;
	_bool m_isShieldReturn = false;
	_bool m_isLootShield = false;
	_bool m_isCoolTime_ShieldThrow = false;
	_float m_fBlockTime = 0.f;

private: /* For.JumpNode */
	CJumpNode* m_pTargetJumpNode = nullptr;
	CJumpTrigger* m_pTargetJumpTrigger = nullptr;
	CJumpBox* m_pTargetJumpBox = nullptr;
	CUI_Blank_CKey* m_pBlankCKey = nullptr;
	CUI_Fill_Ckey* m_pFillCKey = nullptr;
	_float m_fJumpNodeLookTime = 0.f;
	_float m_fJumpTriggerLookTime = 0.f;
	
private: /* For.DropBox  */
	_bool m_isBoxOpen = false;
	CDropBox* m_pTargetDropBox = nullptr;
	CUI_Fill_Ckey2* m_pFillCKey2 = nullptr;

private: /* For.Execution */
	CActor* m_pTargetExecution = nullptr;
	CHierarchyNode* m_pExecutionTargetBone = nullptr;
	CUI_Blank_FKey* m_pBlankFKey = nullptr;

private: /* For.Skill */
	_bool m_isSkill = false;
	_float m_fSkillGuage = 0.f;

private: /* For.Cheat */
	_bool m_isHighSpeedMode = false;

private: _float4  m_color = _float4(0.498f, 0.9411f, 0.8196f, 0.f); // color

private:/* for. Player& Inventory& Equipment Data */
	CPlayerData*	m_pPlayerData = nullptr;
	CInventoryData* m_pInventoryData = nullptr;
	CEquipmentData* m_pEquipmentData = nullptr;

private:
	LIGHTDESC	m_LightDesc;
	_vector		m_lightcolor = XMVectorZero();

private:
	CTexture*		m_pTexture = nullptr;
	vector<CGameObject*>	m_vecMotionTrail;
	CModel*			m_pHealSphere = nullptr;
	_bool			m_bhealcheck = false;
	CHierarchyNode* m_pWeaponBone = nullptr;

public:
	static CSilvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END
#pragma once
#ifndef __METEOR_H__
#define __METEOR_H__

BEGIN(Engine)
class CModel;
class CSphereCollider;
END
BEGIN(Client)
class CMeteor final : public CGameObject
{
private:
	explicit CMeteor();
	explicit CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeteor(const CMeteor& rhs);
	virtual ~CMeteor() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	HRESULT Render_Velocity();
public:
	_int Move(_fvector vPos, _uint iNum);
	_int Shot();
private:
	HRESULT Ready_Component();
private:
	_bool Find_HitPlayer(vector<CGameObject*>* pVecActor);
public:
	static CMeteor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg = nullptr) override;
private:
	virtual void Free() override;
private:
	CModel* m_pStaticModel;
	CModel* m_pAnimModel;
	CSphereCollider* m_pCollider;
private:
	_float m_fSpeed;
	_float4 m_vDestination;
	_bool m_bRemoveCheck;
private:
	_float m_fAccTime;
	_float m_fRandSpawnTime;
	_bool m_bStart;
private:
	_float m_fAccRotateTime;
	_float m_fAccGravityTime;
	_float4 m_vRandNorm;
	_float m_fPreY;
private:
	_float m_fAccMotionTim;
};
END
#endif
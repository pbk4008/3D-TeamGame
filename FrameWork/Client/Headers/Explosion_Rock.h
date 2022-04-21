#pragma once
#ifndef __Explosion_Rock_H__
#define __Explosion_Rock_H__

BEGIN(Engine)
class CModel;
END
BEGIN(Client)
class CExplosion_Rock final : public CEffect
{
public:
	typedef struct tagRockInfo
	{
		_tchar		ModelTag[MAX_PATH]; //파일이름
		_float3		ModelSize;
	}ROCKINFO;
private:
	explicit CExplosion_Rock();
	explicit CExplosion_Rock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CExplosion_Rock(const CExplosion_Rock& rhs);
	virtual ~CExplosion_Rock() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Velocity()  override;

private:
	virtual CEffect* Copy() override;
	virtual void Set_Reset(_bool bReset);

private:
	HRESULT Ready_Component();
	
private:
	ROCKINFO m_Desc;
public:
	static CExplosion_Rock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CEffect* Clone(const _uint iSceneID, void* pArg = nullptr) override;
private:
	virtual void Free() override;
private:
	CModel* m_pAnimModel;
};
END
#endif
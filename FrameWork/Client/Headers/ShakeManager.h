#pragma once

#include "SingleTon.h"
#include "CameraShake.h"

BEGIN(Client)

class CShakeManager : public CSingleTon<CShakeManager>
{
	friend CSingleTon;
public:
	enum class ETemplate{ TestX, TestY, TestZ, TestXY, TestXZ, TestYZ, TestXYZ, MonsterDeath, Max};

private:
	NO_COPY(CShakeManager);
	explicit CShakeManager();
	virtual ~CShakeManager() = default;

public:
	HRESULT NativeConstruct();
	HRESULT Ready_Template();

	void setTemplate(const ETemplate _eTemplate, const CCameraShake::SHAKEEVENT& _tShakeEvent);
	void setOriginCameraShake(CCameraShake* _pOrigin);
	void Shake(const CCameraShake::SHAKEEVENT& _tShakeEvent, const _float3& _vEventPos);
	void Shake(const CCameraShake::SHAKEEVENT& _tShakeEvent, const _fvector& _svEventPos);
	void Shake(const ETemplate _eTemplate, const _float3& _vEventPos);
	void Shake(const ETemplate _eTemplate, const _fvector& _svEventPos);
	void StopShake();

private:
	CCameraShake* m_pOrigin = nullptr;
	CCameraShake::SHAKEEVENT m_arrShakeEvents[(_uint)ETemplate::Max];

public:
	virtual void Free() override;
};

END
#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance_Explosion final : public CVIBuffer
{
	enum class AXIS { AXIS_X,AXIS_Y,AXIS_Z, AXIS_ALL, AXIS_END };
public:
	typedef struct tagPInstanceDesc
	{
		_tchar		ShaderFilePath[MAX_PATH];
		_matrix		matParticle;
		_float3		fParticleStartRandomPos;
		_float3		fParticleMinusRandomDir;
		_float3		fParticleRandomDir;
		_float2		fParticleSize;
		_float		fParticleSpeed;
		_uint		iNumInstance;
	}PIDESC;
protected:
	explicit CVIBuffer_PointInstance_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_PointInstance_Explosion(const CVIBuffer_PointInstance_Explosion& rhs);
	virtual ~CVIBuffer_PointInstance_Explosion() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(/*const _tchar* pShaderFilePath, _uint iNumInstance*/) override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex);

public:
	void Update(_double TimeDelta, _uint eAxis);

public:
	void Particle_Setting_RandomPos();
	void Particle_Reset();

public:
	void Set_Dir(_vector vDir) { XMStoreFloat3(&m_fCamLookDir, vDir); }
	void Set_Desc(PIDESC Desc) { memcpy(&m_Desc, &Desc, sizeof(PIDESC)); }

private:
	//_uint			m_iNumInstance = 0;

	wstring m_wstrShaderFilePath = L"";

	ID3D11Buffer*				m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubresourceData;
	_uint						m_iInstStride = 0; /*정점하나의 바이트 크기. */
	_uint						m_iInstNumVertices = 0;

private:
	PIDESC		m_Desc;
	_float3		m_fCamLookDir;

	_float3* m_pRandomPos = nullptr;
	_float3* m_pDir = nullptr;
	_float3* m_pNormal = nullptr;
	
	_bool m_bReset = false;
	_bool m_bSettingDir = false;
public:
	static CVIBuffer_PointInstance_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext/*, const _tchar* pShaderFilePath, _uint iNumInstance*/);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
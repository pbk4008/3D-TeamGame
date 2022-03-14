#pragma once

BEGIN(Tool)

class CLoader : public CBase
{
private:
	explicit CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CriticalSection() const;
	const ELevel Get_NextLevelID() const;
	const _bool Get_Finished() const;

public:
	HRESULT NativeConstruct(ELevel _eLevel);
	HRESULT Loading_ForAnimationTool();


private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_CS;

	_bool m_isFinished = false;
	ELevel m_eNextLevel = ELevel::Max;

	ID3D11Device* m_pDeivce = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	static CLoader* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const ELevel _eLevel);
	virtual void Free() override;
};

END
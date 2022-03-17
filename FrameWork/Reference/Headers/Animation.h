#pragma once

#include "Base.h"
#include "SaveManager.h"
/* �ִϸ��̼� �ϳ��� ����. */

BEGIN(Engine)
class CAnimation final : public CBase
{
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(char* pName, _double Duration, _double PlaySpeed, const _int _iIndex);
	HRESULT Add_Channel(class CChannel* pChannel);
	HRESULT Update_TransformationMatrix(_double TimeDelta, const _bool _isLoop);
public:
	CChannel* Get_Channel(const char* pChannelName) const;
	const char* Get_Name() const { return m_szName; }
	const _uint Get_Index() const { return m_iIndex; }
	vector <class CChannel*>& Get_Channels() { return m_Channels; }
	const _uint Get_MaxKeyFrameIndex() const;
	const _uint Get_CurrentKeyFrameIndex();
	const _float Get_PalySpeed() const;
	void Set_MaxKeyFrameIndex(const _uint _iMaxKeyFrameIndex);
	const _bool Is_Finished() const { return m_isFinished; }
	CSaveManager::ANIMDATA& SetSaveAnimData();

public:
	void Reset_Animation();
	void Add_TrackAcc(const _double& _dTrackAcc);
private:
	char				m_szName[MAX_PATH] = "";
	_double				m_Duration = 0.0; /*�ϳ��� �ִϸ��̼��� ����ϴµ� �ɸ��� �ð�(Ʈ���� ��ġ�� ����) */
	_double				m_TrackPositionAcc = 0.0;
	_double				m_PlaySpeed = 0.0;
	_bool				m_isFinished = false;

	_uint m_iIndex = 0;
	_uint m_iMaxKeyFrameIndex = 0;
	_uint m_iCurKeyFrameIndex = 0;

private:
	/* ���� �ִϸ��̼ǿ� ���⤷�� �ִ� ������� ����. */
	vector<class CChannel*>			m_Channels; 
	typedef vector<class CChannel*>	CHANNELS;
public:
	static CAnimation* Create(char* pName, _double Duration, _double PlaySpeed, const _int _iIndex);
	CAnimation* Clone();
	virtual void Free();
};

END
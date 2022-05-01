#ifndef VoiceManager_h__
#define VoiceManager_h__

#include "SingleTon.h"

BEGIN(Client)
class CSubtitles;
class CVoiceManager final :  public CSingleTon<CVoiceManager>
{
	friend CSingleTon;

private:
	NO_COPY(CVoiceManager);
	CVoiceManager(void);
	~CVoiceManager(void) = default;

public:
	HRESULT NativeConstruct(SCENEID _iSceneID);
	_int    Tick(_double dTimeDelta);
	_int	Late_Tick(_double dTimeDelta);
	HRESULT	Render(void);

private:
	void Ready_TriggerFile(void);
	void ShowNext(void);

private: /* for. Stage 1 */
	const std::wstring arrFirst[2]
	{
		L"T_Voice_IsYourArmor_Ravenna",
		L"T_Voice_ItsSluggish_Orin",
	};
	const std::wstring arrSecond[2]
	{
		L"T_Voice_SirYoureStill_Ravenna",
		L"T_Voice_ItDoesntMatter_Orin",
	};
	const std::wstring arrThird[2]
	{
		L"T_Voice_SheWontWant_Soras",
		L"T_Voice_IKnow_Orin",
	};
	const std::wstring arrFourth[2]
	{
		L"T_Voice_ImApproachingThe_Orin",
		L"T_Voice_GoodLuckOrin_Ravenna",
	};
	std::wstring m_szTextureName;

private: /* for. Stage 1*/
	const std::wstring arrVoiceFirst[2]
	{
		L"IsYourArmor",
		L"ItsSluggish",
	};
	const std::wstring arrVoiceSecond[2]
	{
		L"SirYoureStill",
		L"ItDoesntMatter",
	};
	const std::wstring arrVoiceThird[2]
	{
		L"SheWontWant",
		L"IKnow",
	};
	const std::wstring arrVoiceFourth[2]
	{
		L"ImApproachingThe",
		L"GoodLuckOrin",
	};
	std::wstring m_szSoundKey;

private: /* for. Stage 2 */
	const std::wstring arrFirst2[4]
	{
		L"T_Voice_TheMonolithIs_Sanctum",
		L"T_Voice_ForNow_Orin",
		L"T_Voice_HeNearlyHas_Sanctum",
		L"T_Voice_iDidntCome_Orin"
	};
	const std::wstring arrSecond2[3]
	{
		L"T_Voice_AndIAm_Sanctum",
		L"T_Voice_WeWillDiscuss_Orin",
		L"T_Voice_TheOddsAre_Sanctum",
	};
	const std::wstring arrThird2[2]
	{
		L"T_Voice_HowCouldYou_Sanctum",
		L"T_Voice_IllFindA_Orin",
	};
	const std::wstring arrFourth2[2]
	{
		L"T_Voice_HowDidYou_Sanctum",
		L"T_Voice_IHaveA_Orin",
	};

	const std::wstring arrFifth2[3]
	{
		L"T_Voice_WhatDoYou_Sanctum",
		L"T_Voice_HeBecomesA_Orin",
		L"T_Voice_IFearYou_Sanctum",
	};

private: /* for. Stage 2*/
	const std::wstring arrVoiceFirst2[4]
	{
		L"TheMonolithIs", 
		L"ForNow",
		L"HeNearlyHas",
		L"iDidntCome",
	};
	const std::wstring arrVoiceSecond2[3]
	{
		L"AndIAm",
		L"WeWillDiscuss",
		L"TheOddsAre",
	};
	const std::wstring arrVoiceThird2[2]
	{
		L"HowCouldYou",
		L"IllFindA",
	};
	const std::wstring arrVoiceFourth2[2]
	{
		L"HowDidYou",
		L"IHaveA",
	};
	const std::wstring arrVoiceFifth2[3]
	{
		L"WhatDoYou",
		L"HeBecomesA",
		L"IFearYou",
	};

private: /* for. Stage 3 */
	const std::wstring arrFirst3[3]
	{
		L"T_Voice_YourOddsAre_Sanctum",
		L"T_Voice_WhatIsIt_Orin",
		L"T_Voice_YouWontLike_Sanctum",
	};

private: /* for. Stage 3*/
	const std::wstring arrVoiceFirst3[3]
	{
		L"YourOddsAre",
		L"WhatIsIt",
		L"YouWontLike",
	};

private:
	std::vector<CSubtitles*> m_vecSubtiltesTex;
	_uint					 m_arrStage1[4] = { 2, 2, 2, 2 };
	_uint					 m_arrStage2[5] = { 4, 3, 2, 2, 3 };
	_uint					 m_arrStage3[1] = { 3 };

	_uint					 m_iCurrentTrigerrIdx = 0;
	_uint					 m_iTexIdx = 0;
	_int					 m_iSSceneID = 0;

private:
	_bool	m_bSubtileShow = false;
	_bool	m_bIsPlaying   = true;
	_bool   m_bPlayOne	   = false;

	CSoundMgr::CHANNELID m_EChannel = CSoundMgr::CHANNELID::MAXCHANNEL;
	std::wstring szTemp = L"";

private:
	virtual void Free() override;
};
END

#endif // VoiceManager_h__
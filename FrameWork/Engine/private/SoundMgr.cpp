#include "SoundMgr.h"

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
{
	for (auto& i : m_pChannelArr)
		i = nullptr;
}

HRESULT CSoundMgr::Init_SoundManager()
{
	FMOD_RESULT hr = FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	if (hr != FMOD_OK)
		return E_FAIL;
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	if (FMOD_OK != FMOD_System_Init(m_pSystem, 512, FMOD_INIT_NORMAL, NULL))
		return E_FAIL;

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSoundMgr::Add_Sound(const wstring& pSoundTag, const char* pSoundPath)
{
	FMOD_SOUND* pSound = nullptr;

	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, pSoundPath, FMOD_LOOP_OFF, 0, &pSound);

	if (eRes == FMOD_OK)
		m_mapSound.emplace(pSoundTag, pSound);

	return S_OK;
}

void CSoundMgr::Play_Shot(const wstring& pSoundKey, CHANNELID eID)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE; 
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(const wstring& pSoundKey)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem,iter->second, nullptr, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::VolumeChange(CHANNELID eID, _float fVolume)
{
	FMOD_BOOL bPlay = false;

	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);
	
	if(bPlay)
		FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
}

void CSoundMgr::BlendSound(const std::wstring& pStartSoundKey, 
						   const std::wstring& pEndSoundKey,
						   CHANNELID eStartID,
						   CHANNELID eEndID,
						  _float fBlendTiming)
{
	auto Startiter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pStartSoundKey));
	auto Enditer = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pEndSoundKey));

	if (Startiter == m_mapSound.end()
		||
		Enditer == m_mapSound.end())
		PRINT_LOG(L"Waring", L"지정한 사운드키를 찾을 수 없어요");

	StopSound(eStartID);
	StopSound(eEndID);

	FMOD_RESULT Result = FMOD_ERR_FILE_BAD;
	FMOD_BOOL bPlay = false;
	_uint Position = 0.f, Len = 0.f;
	FMOD_Sound_GetLength(Startiter->second, &Len, FMOD_TIMEUNIT_MS);

	Play_Shot(pStartSoundKey, eStartID);
	
	FMOD_Channel_IsPlaying(m_pChannelArr[eStartID], &bPlay);

	if(bPlay)
		Play_Shot(pEndSoundKey, eEndID);
}

HRESULT CSoundMgr::LoadSoundFile()
{
	// BGM
	FMOD_SOUND* pSound = nullptr;
	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem,
		"../bin/Resources/Sound/Bgm/BM_ZU_AveyronPeak_78_v3_body_38bars.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage1_BGM", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Bgm/BM_DynamicFightMusic081619_Loop_v2_body.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage1_Boss_BGM", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Short_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Swing_Long_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_4", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Power_Resonance_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_L_4_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Unsheath_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_UnEquip", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Weapon_Sword_1H_Unsheath_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Equip", pSound); else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Blind_Attack_Cast_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_P1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Polearm_Heavy_Finisher_Combo_04_fr8.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_P2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Ultimate_Sword_ChargeUp_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Greatsword_Throw_03_fr10.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Weapon_Longsword_Slash_Heavy_A_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Greatsword_R2_Charged_Slash_Swing_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_Relese", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Weapon_Longsword_Slash_Heavy_Charged_Buildup_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_Slam_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Weapon_Longsword_SpiralTechnique_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_Slam_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Needle/A_Player_Weapon_Longsword_Slash_Heavy_ChargingFinished_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Needle_Attack_R_Slam_3", pSound); else return E_FAIL;

	/* for. Player */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Heavy_Armour_Plate_Gak_Step_10.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Step_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Zamora_Foley_Dash_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Dash", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Heavy_Armour_Cloth_Step_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Dash_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_LowHealth_01.ogg", FMOD_LOOP_NORMAL, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_LowHealth", pSound); else return E_FAIL;

	/* for. Shield*/
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Weapon_Shield_Equip_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_On", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Weapon_Shield_Stow_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Off", pSound); else return E_FAIL;

	/* for.Monster */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_Attack_Liquid_Hit_Generic_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_Weapon_Hit_Damage_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_Weapon_Hit_Damage_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_Weapon_Hit_Damage_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit_4", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Player_Takes_Hit_A_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit_5", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Bastion/A_Weapon_Generic_Critical_Hit_OLD_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit_6", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/A_LootExplosion_Element_Cold_08.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Death", pSound); else return E_FAIL;

	/* for. Crawler */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Crawler/A_Vox_CrystalCrawler_Attack_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Crawler_Attack_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Crawler/A_Vox_CrystalCrawler_Attack_06.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Crawler_Attack_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Crawler/A_Weapon_Spectral_Hit_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Crawler_Attack_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Crawler/A_Vox_CrystalCrawler_Hit_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Crawler_Death", pSound); else return E_FAIL;

	/* for. Earth */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/A_Vox_AncientGlaive_Death_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Death", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/A_Overhead_Axe_Swing_Decimus_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Attack", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/VO_EFF_602_AbyssianTribe_Growl_09.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Attack_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/VO_EFF_602_AbyssianTribe_Growl_06.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Attack_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/A_Foley_SunChanneler_Dash_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Dash_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Earth/A_Attack_Dash_Rattle_Rancor_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Earth_Dash_2", pSound); else return E_FAIL;

	/* for. Bastion */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Bastion/A_AirRippler_Death_ArmourFall_05.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Bastion_Death", pSound); else return E_FAIL;

	/* Healer */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Healer/A_Vox_Hit_NyakCaster_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Healer_Death", pSound); else return E_FAIL;

	/* H1_Sword */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/H1_Sword/A_Player_Weapon_Longsword_Slash_Leaping_Charge_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"H1_Attack_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/H1_Sword/A_Weapon_Sword_1H_Swing_Long_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"H1_Attack_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/H1_Sword/A_Weapon_Sword_1H_Swing_Short_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"H1_Attack_3", pSound); else return E_FAIL;

	/* Shooter */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Shooter/A_AbyssianSpitter_Attack_Spit_Shot_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shooter_Shot", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Shooter/A_CrimsonMarksman_Crossbow_Reload_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shooter_Reload", pSound); else return E_FAIL;

	/* Spear */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/VO_EFF_583_Decimus_Roar_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Roar", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/c2290_swing2_wav.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Swing", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/A_Weapon_Shield_Jab_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Guard", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/A_Weapon_Blade_Hit_Plate_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Guard_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/A_Nyak_Attack_Polearm_Whoosh_Short_06.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Attack_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/A_Player_Polearm_Catch_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Attack_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/Spear/A_AbyssianLancer_Attack_Polearm_Blade_Smash_06.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Spear_Attack_3", pSound); else return E_FAIL;

	/* for. Mid Boss*/
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_589_Excelcius_Roar_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Roar", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_630_VargulTribe_Death_05.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Death", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_586_Excelcius_Attack_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Roar_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_553_Macros_Roar_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Roar_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_613_CrimsonWindTribe_Roar_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Roar_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_586_Excelcius_Attack_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/VO_EFF_586_Excelcius_Attack_06.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/A_Excelsious_Foley_Step_07.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Step", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Phalanxar_Attack_Hammer_Swing_Long_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Swing_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Phalanxar_Attack_Hammer_Swing_Short_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Swing_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Player_Warhammer_Concentrated_Blast_Slam_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack_Down", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/A_LootExplosion_Element_Lightning_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Explode", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Monster/MidBoss/A_Zamora_Attack_EnrageRitual_Complete_Explode_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Explode_End", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Player_Warhammer_R1_Impact_01a_fr31.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack_End_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Weapon_Impact_Hammer_Critical_Flesh_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack_End_2", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Phalanxar_Attack_Hammer_Slam_Impact_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Attack_End_3", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/MidBoss/A_Foley_Tuskboar_Jump_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"MidBoss_Jump", pSound); else return E_FAIL;

	/* for.Jump Node */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Effect_JumpNode_Open_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Open", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Effect_JumpNode_Close_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Close", pSound); else return E_FAIL;

	FMOD_System_Update(m_pSystem);

	return S_OK;
}


void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
		FMOD_Sound_Release(Mypair.second);

	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

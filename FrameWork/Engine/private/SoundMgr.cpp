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
	{
		MSGBOX(L"사운드 파일 로드 실패 경로와 파일을 확인하세용");
		return E_FAIL;
	}
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

_bool CSoundMgr::IsPlaying(CHANNELID eID)
{
	FMOD_BOOL bPlay = false;
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);

	return bPlay;
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
	_uint Position = 0, Len = 0;
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

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Bgm/A_Amb_SunforgeTemple_Bed.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage2_BGM", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Bgm/BM_InterfaceThemeAmbient_112_66bars_v4.ogg", FMOD_LOOP_NORMAL, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Logo_BGM", pSound); else return E_FAIL;

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

	/* for. Item */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Item/A_Foley_GetUp_SunWisp_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Drop_Item", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Item/A_UI_Hover_Major.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Drop_Item_2", pSound); else return E_FAIL;

	/* for. Player *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Item/A_Loot_Chest_Unlock_Complete_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Unlock_DropBox", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Item/A_Loot_Legendary_Pickup_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Item_PickUp", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Item/GF_Anvil_Charged_Loop_v1.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Opeing_DropBox", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Heavy_Armour_Plate_Gak_Step_10.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Step_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Zamora_Foley_Dash_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Dash", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Heavy_Armour_Cloth_Step_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Dash_1", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_LowHealth_01.ogg", FMOD_LOOP_NORMAL, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_LowHealth", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Gap_Closer_Rushing_Strike_Stab_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Gap_Closer", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/Jump.mp3", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Skill_3_Jump", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Attack_Windhowler_Dash_Leap_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Dash_Leap_01", pSound); else return E_FAIL;

	/* For.Shield *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Phalanxar_Consume_Heal_Chug_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Heal", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Archon_Friendly_Heal_Loop_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Heal_Start", pSound); else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Weapon_Swap_Hammer_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Swap_Hammer", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Weapon_Swap_Longsword_03.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Swap_Longsword", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Footstep_BrokenStone_Scuff_04.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Walk", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/A_Player_Foley_GapJump_Land_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_GapJump", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Player/Player_Knockback_A_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Knockback", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Weapon_Shield_Equip_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_On", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Weapon_Shield_Stow_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Off", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_UI_HUD_Shield_Recharge_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Recharge", pSound); else  return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Shield_Catch_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Catch", pSound); else  return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Player_Shield_Charge_Start_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Charge", pSound); else  return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Weapon/Shield/A_Weapon_Shield_Block_Longsword_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Block", pSound); else  return E_FAIL;

	/* For.Hammer *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Warhammer_R1_Impact_01a_fr31.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Warhammer_R1_Impact_01", pSound); else return E_FAIL;
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Warhammer_R1_Swing_01b_fr25.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Warhammer_R1_Swing_01", pSound); else return E_FAIL;
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Weapon_Hammer_Swing_Long_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Hammer_Swing", pSound); else return E_FAIL;
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Warhammer_Running_Slam_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Warhammer_Running_Slam", pSound); else return E_FAIL;
	// JustFrame
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Warhammer_JustFrame_Catch_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JustFrame_Catch_01", pSound); else return E_FAIL;
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Warhammer_JustFrame_Throw_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JustFrame_Throw_01", pSound); else return E_FAIL;
	// Charge
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Weapon_Hammer_L2_ChargeBuild_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Hammer_L2_ChargeBuild", pSound); else return E_FAIL;
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Hammer/A_Player_Weapon_Hammer_L2_ChargeIgnite_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Hammer_L2_ChargeIgnite", pSound); else return E_FAIL;

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

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Player_JumpNode_Flying_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Flying", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/JumpNode/A_Player_JumpNode_Flying_02.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JumpNode_Flying_2", pSound); else return E_FAIL;

	/*for. Voice Orin */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a1_003b_Orin_ItsSluggish.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ItsSluggish", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SI3a1_028b_Orin_ItDoesntMatter.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ItDoesntMatter", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a1_003g_Orin_IKnow.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IKnow", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a1_003h_Orin_ImApproachingThe.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ImApproachingThe", pSound); else return E_FAIL;
	//2 Stage
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006b_Orin_ForNow.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ForNow", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006d_Orin_iDidntCome.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"iDidntCome", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006g_Orin_WeWillDiscuss.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"WeWillDiscuss", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006k_Orin_IHaveA.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IHaveA", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006i_Orin_IllFindA.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IllFindA", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_006m_Orin_HeBecomesA.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"HeBecomesA", pSound); else return E_FAIL;
	//3 Stage
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Orin/VO_SM1a2_007b_Orin_WhatIsIt.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"WhatIsIt", pSound); else  return E_FAIL;

	/*for. Voice Ravenna */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Ravenna/VO_SM1a1_003a_Ravenna_IsYourArmor.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IsYourArmor", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Ravenna/VO_SM1a1_003d_Ravenna_SirYoureStill.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"SirYoureStill", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Ravenna/VO_SM1a1_003i_Ravenna_GoodLuckOrin.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"GoodLuckOrin", pSound); else return E_FAIL;

	/*for. Voice Soras */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Soras/VO_SM1a1_003f_Soras_SheWontWant.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"SheWontWant", pSound); else return E_FAIL;

	/* for. Sanctum */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006a_SeventhSanctum_TheMonolithIs.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"TheMonolithIs", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006c_SeventhSanctum_HeNearlyHas.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"HeNearlyHas", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006e_SeventhSanctum_TheOddsAre.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"TheOddsAre", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006f_SeventhSanctum_AndIAm.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"AndIAm", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006j_SeventhSanctum_HowDidYou.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"HowDidYou", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006h_SeventhSanctum_HowCouldYou.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"HowCouldYou", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006l_SeventhSanctum_WhatDoYou.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"WhatDoYou", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_006n_SeventhSanctum_IFearYou.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IFearYou", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_007a_SeventhSanctum_YourOddsAre.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"YourOddsAre", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Voice/Sanctum/VO_SM1a2_007c_SeventhSanctum_YouWontLike.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"YouWontLike", pSound); else return E_FAIL;

	/* for. UI */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/UI/A_UI_tab_over.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"UI_tab_over", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/UI/A_Loot_Chest_Open_Locked_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"UI_InvenOpen", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/UI/A_UI_transition_left_in.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"UI_Equip", pSound); else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/UI/A_Show_GuideImg.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"UI_GuideImg", pSound); else return E_FAIL;

	/* for. Monster Battle */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/MonsterBattle/BM_CD_SunforgeDrums02_240_v4_body_40bars.ogg", FMOD_LOOP_NORMAL, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Battle", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/MonsterBattle/BM_CD_CombatDrums03_136_v4_body_40bars.ogg", FMOD_LOOP_NORMAL, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Battle_2", pSound); else return E_FAIL;

	/* For.Boss *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region 보스 사운드들
	// Beam
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_ChargeShot_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_ChargeShot_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_End_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_End_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_FlyUp_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_FlyUp_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_Loop.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_Loop", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_Shoot_Explode_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_Shoot_Explode_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_BeamMassacre_Start_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BeamMassacre_Start_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// FistSmash
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_FistSmash_Charge_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"FistSmash_Charge_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// GapClose
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_GapClose_Land_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"GapClose_Land_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_GapClose_PreLand_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"GapClose_PreLand_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// OverloadProtocol
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_OverloadProtocol_Charge_Start_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"OverloadProtocol_Charge_Start_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_OverloadProtocol_Explode_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"OverloadProtocol_Explode_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// RageOrb
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_RageOrb_ChargeShot_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"RageOrb_ChargeShot_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_RageOrb_Shoot_Explode_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"RageOrb_Shoot_Explode_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// StrafeLazer
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Attack_StrafeLazer_Explode_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"StrafeLazer_Explode_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// Intermission
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Intermission_Hit_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Intermission_Hit_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Intermission_RageRestored_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Intermission_RageRestored_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Intermission_ShieldRestored_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Intermission_ShieldRestored_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Intermission_Start_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Intermission_Start_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// Shield
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Shield_Destroyed_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Destroyed_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Solaris_Shield_Hit_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Shield_Hit_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
	// Hit
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Boss/A_Weapon_Impact_Longsword_v_Metal_01.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Longsword_v_Metal_01", pSound); else MSGBOX(L"보스 사운드 로드 실패");
#pragma endregion

	/* for. Cinema */
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot1/VO_SM1a1_002a_Soras_ThisWontBe.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ThisWontBe", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot1/VO_SM1a1_002b_Ravenna_IfAnyoneCan.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"IfAnyoneCan", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot2/VO_SM1a1_002c_Soras_Sir.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Sir", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot2/VO_SM1a1_002d_Orin_YouUnderstandHow.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"YouUnderstandHow", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot2/VO_SM1a1_002e_Ravenna_YouCanCount.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"YouCanCount", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot2/VO_SM1a1_002f_Orin_YouTwoCover.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"YouTwoCover", pSound); else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot3/VO_SM1a1_004a_Phalanxar_NoManShall.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"NoManShall", pSound); else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../bin/Resources/Sound/Cinema/Shot3/VO_SM1a1_004d_Phalanxar_Auuugh.ogg", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Auuugh", pSound); else return E_FAIL;

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

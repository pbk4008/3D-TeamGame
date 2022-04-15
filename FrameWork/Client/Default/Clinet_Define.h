#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1280;
const unsigned int g_iWinCy = 720;
const float MAXGROOGUGAGUE  = 10.0f;

#define HALF_WINCX 640
#define HALF_WINCY 360
#define Max_InventorySlot 32
#define Max_EquipmentSlot 8
const wstring g_arrGradeName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"ÀÏ¹Ý",
	L"°í±Þ",
	L"Èñ±Í",
	L"¿µ¿õ",
	L"Àü¼³"
};
static TEXTURETYPE g_arrMI_Texture[(int)EWeaponMaterial::MATERIAL_END]
{
	TEXTURETYPE::TEX_DIFFUSE,
	TEXTURETYPE::TEX_NORMAL,
	TEXTURETYPE::TEX_MRA,
	TEXTURETYPE::TEX_CEO,
	TEXTURETYPE::TEX_MASK
};

static const string g_arrMI_Tex[(int)EWeaponMaterial::MATERIAL_END]
{
	"g_DiffuseTexture",
	"g_BiNormalTexture",
	"g_MRATexture",
	"g_CEOTexture"
	"g_MASKTexture"
};

const wstring g_arrGradeSlotBgName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_Item_Bg_Grey",
	L"T_Item_Bg_Blue",
	L"T_Item_Bg_Green",
	L"T_Item_Bg_Purple",
	L"T_Item_Bg_Orange"
};

const wstring g_arrGradeStatusBgName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_ItemStatus_Bg_Common",
	L"T_ItemStatus_Bg_Rare",
	L"T_ItemStatus_Bg_Uncommon",
	L"T_ItemStatus_Bg_Epic",
	L"T_ItemStatus_Bg_Legendary"
};

const wstring g_arrGradeEffectName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_Item_Glow_Grey",
	L"T_Item_Glow_Blue",
	L"T_Item_Glow_Green",
	L"T_Item_Glow_Purple",
	L"T_Item_Glow_Orange"
};

const wstring g_arrMI_ArgentBlade[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_Sword_ArgentBlade_D.dds",
	L"T_1H_Sword_ArgentBlade_N.dds",
	L"T_1H_sword_ArgentBlade_MRA.dds",
};

const wstring g_arrMI_Crossblade[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1h_Sword_Crossblade_D.dds",
	L"T_1h_Sword_Crossblade_N.dds",
	L"T_1h_Sword_Crossblade_MRA.dds",
};

const wstring g_arrMI_Dragon[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1h_Sword_Dragon_D.dds",
	L"T_1h_Sword_Dragon_N.dds",
	L"T_1h_Sword_Dragon_MRA.dds",
	L"T_1h_Sword_Dragon_CEO.dds"
};

const wstring g_arrMI_Eclipse[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_Sword_Eclipse_Gold_D.dds",
	L"T_1H_Sword_Eclipse_Gold_N.dds",
	L"T_1H_Sword_Eclipse_Gold_MRA.dds",
	L"T_1H_Sword_Eclipse_Gold_CEO.dds"
};

const wstring g_arrMI_Justice[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_sword_Justice_D.dds",
	L"T_1H_sword_Justice_N.dds",
	L"T_1H_sword_Justice_MRA.dds",
	L"T_1H_sword_Justice_CEO.dds"
};

const wstring g_arrMI_GodHammer[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Godhammer_D.dds",
	L"T_2H_hammer_Godhammer_N.dds",
	L"T_2H_hammer_Godhammer_MRA.dds",
};

const wstring g_arrMI_Harbinger[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_Hammer_Harbinger_D.dds",
	L"T_2h_Hammer_Harbinger_N.dds",
	L"T_2h_Hammer_Harbinger_MRA.dds",
	L"T_2h_Hammer_Harbinger_CEO.dds"
};

const wstring g_arrMI_Hinterclaw[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Hinterclaw_D.dds",
	L"T_2H_hammer_Hinterclaw_N.dds",
	L"T_2H_hammer_Hinterclaw_MRA.dds",
	L"T_2H_hammer_Hinterclaw_CEO.dds"
};

const wstring g_arrMI_Ironstone[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Ironstone_D.dds",
	L"T_2h_hammer_Ironstone_N.dds",
	L"T_2h_hammer_Ironstone_MRA.dds",
	L"T_2h_hammer_Ironstone_CEO.dds"
};

const wstring g_arrMI_Legend1[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend1_D.dds",
	L"T_2H_hammer_Legend1_N.dds",
	L"T_2H_hammer_Legend1_MRA.dds",
	L"T_2H_hammer_Legend1_CEO.dds",
	L"T_2H_hammer_Legend1_MASK.dds"
};

const wstring g_arrMI_Legend2[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend2_D.dds",
	L"T_2H_hammer_Legend2_N.dds",
	L"T_2H_hammer_Legend2_MRA.dds",
	L"T_2H_hammer_Legend2_CEO.dds",
};

const wstring g_arrMI_Legend5[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend5_D.dds",
	L"T_2H_hammer_Legend5_N.dds",
	L"T_2H_hammer_Legend5_MRA.dds",
	L"T_2H_hammer_Legend5_CEO.dds",
};

const wstring g_arrMI_Mesa[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Mesa_D.dds",
	L"T_2h_hammer_Mesa_N.dds",
	L"T_2h_hammer_Mesa_MRA.dds",
	L"T_2h_hammer_Mesa_CEO.dds",
};

const wstring g_arrMI_Skymourne[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Skymourne_D.dds",
	L"T_2h_hammer_Skymourne_N.dds",
	L"T_2h_hammer_Skymourne_MRA.dds",
	L"T_2h_hammer_Skymourne_CEO.dds",
};

const wstring g_arrMI_Soothsayer[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_Hammer_Soothsayer_D.dds",
	L"T_2H_Hammer_Soothsayer_N.dds",
	L"T_2H_Hammer_Soothsayer_MRA.dds",
	L"T_2H_Hammer_Soothsayer_CEO.dds",
};


extern HWND g_hWnd;
extern HINSTANCE  g_hInst;

extern CMainApp* g_pMainApp;
extern CClient_Observer* g_pObserver;
extern CDebugSystem* g_pDebugSystem;
extern CShakeManager* g_pShakeManager;
extern CDataManager*  g_pDataManager;
extern CInven_UIManager* g_pInvenUIManager;
extern CInteractManager* g_pInteractManager;
extern CDropManager* g_pDropManager;
extern CWeaponGenerator* g_pWeaponGenerator;

#endif
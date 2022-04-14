#include "pch.h"
#include "WeaponGenerator.h"
#include "Weapon.h"
#include "HierarchyNode.h"
#include "Needle.h"
#include "Fury.h"
#include "Player_Weapon.h"
CWeaponGenerator::CWeaponGenerator(void)
{
}

HRESULT CWeaponGenerator::NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _uint _iSceneID, CModel* _pModel)
{
	CHierarchyNode* pHierarchyNode  = _pModel->Get_BoneMatrix("spine_03");
	CPlayer_Weapon::Desc desc;
	CWeaponData::Desc weaponDataDesc;
	{
		// ÇÑ¼Õ°Ë
		weaponDataDesc.weaponName = L"Needle";
		weaponDataDesc.damage = 20.f;
		weaponDataDesc.weaponType = EWeaponType::LongSword;
		CWeaponData weaponData(weaponDataDesc);

		CWeapon* pWeapon = CNeedle::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, pHierarchyNode);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());
		
		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		// Fury
		weaponDataDesc.weaponName = L"Fury";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		CWeapon* pWeapon = CFury::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, pHierarchyNode);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//ArgentBlade
		weaponDataDesc.weaponName = L"ArgentBlade";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::LongSword;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"ArgentBlade";
		desc.modelName = L"Model_ArgentBlade";
		desc.arrMaterial = g_arrMI_ArgentBlade;
		desc.damage = 20.f;
		desc.NumMat = 3;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Sword_1H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Eclipse
		weaponDataDesc.weaponName = L"Eclipse";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::LongSword;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Eclipse";
		desc.modelName = L"Model_Eclipse";
		desc.arrMaterial = g_arrMI_Eclipse;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Sword_1H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}

	{
		//Dragon
		weaponDataDesc.weaponName = L"Dragon";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::LongSword;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Dragon";
		desc.modelName = L"Model_Dragon";
		desc.arrMaterial = g_arrMI_Dragon;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Sword_1H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}

	{
		//Crossblade
		weaponDataDesc.weaponName = L"Crossblade";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::LongSword;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Crossblade";
		desc.modelName = L"Model_Crossblade";
		desc.arrMaterial = g_arrMI_Crossblade;
		desc.damage = 20.f;
		desc.NumMat = 3;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Sword_1H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Justice
		weaponDataDesc.weaponName = L"Justice";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Justice";
		desc.modelName = L"Model_Justice";
		desc.arrMaterial = g_arrMI_Justice;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Sword_1H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));

	}
	{
		//Legend1
		weaponDataDesc.weaponName = L"Legend1";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Legend1";
		desc.modelName = L"Model_Legend1";
		desc.arrMaterial = g_arrMI_Legend1;
		desc.damage = 20.f;
		desc.NumMat = 5;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;
		desc.weaponColor = { 0.07f, 0.439f, 0.76f, 1.f };
		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Ironstone
		weaponDataDesc.weaponName = L"Ironstone";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Ironstone";
		desc.modelName = L"Model_Ironstone";
		desc.arrMaterial = g_arrMI_Ironstone;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Skymourne
		weaponDataDesc.weaponName = L"Skymourne";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Skymourne";
		desc.modelName = L"Model_Skymourne";
		desc.arrMaterial = g_arrMI_Skymourne;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Soothsayer
		weaponDataDesc.weaponName = L"Soothsayer";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Soothsayer";
		desc.modelName = L"Model_Soothsayer";
		desc.arrMaterial = g_arrMI_Soothsayer;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Mesa
		weaponDataDesc.weaponName = L"Mesa";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Mesa";
		desc.modelName = L"Model_Mesa";
		desc.arrMaterial = g_arrMI_Mesa;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Legend5
		weaponDataDesc.weaponName = L"Legend5";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Legend5";
		desc.modelName = L"Model_Legend5";
		desc.arrMaterial = g_arrMI_Legend5;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}

	{
		//Legend2
		weaponDataDesc.weaponName = L"Legend2";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Legend2";
		desc.modelName = L"Model_Legend2";
		desc.arrMaterial = g_arrMI_Legend2;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Hinterclaw
		weaponDataDesc.weaponName = L"Hinterclaw";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Hinterclaw";
		desc.modelName = L"Model_Hinterclaw";
		desc.arrMaterial = g_arrMI_Hinterclaw;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//Harbinge
		weaponDataDesc.weaponName = L"Harbinger";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"Harbinger";
		desc.modelName = L"Model_Harbinger";
		desc.arrMaterial = g_arrMI_Harbinger;
		desc.damage = 20.f;
		desc.NumMat = 4;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	{
		//GodHammer
		weaponDataDesc.weaponName = L"GodHammer";
		weaponDataDesc.damage = 30.f;
		weaponDataDesc.weaponType = EWeaponType::Hammer;
		CWeaponData weaponData(weaponDataDesc);

		desc.weaponName = L"GodHammer";
		desc.modelName = L"Model_GodHammer";
		desc.arrMaterial = g_arrMI_GodHammer;
		desc.damage = 20.f;
		desc.NumMat = 3;
		desc.pHierarchyNode = pHierarchyNode;
		desc.EWeaponType = CWeapon::EType::Hammer_2H;

		CPlayer_Weapon* pWeapon = CPlayer_Weapon::Create(pDevice, pDeviceContext);
		pWeapon->NativeConstruct(_iSceneID, desc);
		pWeapon->Set_OwnerPivotMatrix(_pModel->Get_PivotMatrix());

		m_vecWeapons.push_back(std::make_pair(weaponData, pWeapon));
	}
	return S_OK;
}

CWeapon* CWeaponGenerator::GetWeapon(std::wstring weaponName)
{
	for (auto weapon : m_vecWeapons)
	{
		if (0 == weapon.second->Get_Name().compare(weaponName))
			return weapon.second;
	}
	assert(false);
	return nullptr;
}

CWeaponData CWeaponGenerator::GetWeaponData(std::wstring weaponName)
{
	for (auto weapon : m_vecWeapons)
	{
		if (0 == weapon.second->Get_Name().compare(weaponName))
			return weapon.first;
	}
	assert(false);
	return CWeaponData();
}

void CWeaponGenerator::Free()
{
	for (auto weapon : m_vecWeapons)
		Safe_Release(weapon.second);

	m_vecWeapons.clear();
}

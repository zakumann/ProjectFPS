// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPistol.h"

AWeaponPistol::AWeaponPistol()
{
	WeaponType = EWeaponType::Pistol;
	WeaponSlot = 2;
	WeaponName = FText::FromString(TEXT("Pistol"));
	HandSocketName = TEXT("PistolGrip");
}

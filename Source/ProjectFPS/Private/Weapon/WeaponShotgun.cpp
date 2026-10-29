// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponShotgun.h"

AWeaponShotgun::AWeaponShotgun()
{
	WeaponType = EWeaponType::Shotgun;
	WeaponSlot = 3;
	WeaponName = FText::FromString(TEXT("Shotgun"));
	HandSocketName = TEXT("ShotgunGrip");
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAssaultRifle.h"

AWeaponAssaultRifle::AWeaponAssaultRifle()
{
	WeaponType = EWeaponType::AssaultRifle;
	WeaponSlot = 4;
	WeaponName = FText::FromString(TEXT("AssaultRifle"));
	HandSocketName = TEXT("RifleGrip");
}

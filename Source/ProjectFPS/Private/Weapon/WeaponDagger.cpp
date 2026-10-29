// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponDagger.h"

AWeaponDagger::AWeaponDagger()
{
	WeaponType = EWeaponType::Dagger;
	WeaponSlot = 1;
	WeaponName = FText::FromString(TEXT("Dagger"));
	HandSocketName = TEXT("KnifeGrip");
}
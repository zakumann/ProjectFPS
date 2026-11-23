// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Pistol.h"
#include "Engine/World.h"

APistol::APistol()
{
	AttachSocketName = FName("WeaponSocket_R");
}

void APistol::Fire()
{
	Super::Fire();

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireRate)
	{
		return;
	}
	LastFireTime = CurrentTime;

	UE_LOG(LogTemp, Log, TEXT("Pistol Fire!"));
}

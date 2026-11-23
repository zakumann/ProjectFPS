// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFPS_API APistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	APistol();

	virtual void Fire() override;

protected:
	/** 권총 고유 스탯 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Stats")
	float FireRate = 0.25f; // 연사 제한 간격(초)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Stats")
	float Damage = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Stats")
	float MaxRange = 5000.f;

private:
	float LastFireTime = -10.f;
};

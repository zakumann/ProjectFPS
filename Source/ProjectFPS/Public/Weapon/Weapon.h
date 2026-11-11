// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTFPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:	
	AWeapon();

	USkeletalMeshComponent* GetMesh1P() const;

	void AttachToOwningPawn() const;

	UPROPERTY(EditAnywhere, Category = "FPS|WeaponType")
	FGameplayTag WeaponType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Weapon Mesh: 1st person view
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	void SetMeshVisibilities(APawn* OwningPawn) const;
};

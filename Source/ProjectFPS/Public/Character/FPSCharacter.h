// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UCombatComponent;

UCLASS()
class PROJECTFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void Input_CycleWeapon();

	void Input_ReloadWeapon();

	void Input_FireWeapon_Pressed();
	void Input_FireWeapon_Released();

	void Input_Aim_Pressed();
	void Input_Aim_Released();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> Combat;

	// 1st person view (arms)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> CycleWeaponAction;

	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> FireWeaponAction;

	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> ReloadWeaponAction;

	UPROPERTY(EditAnywhere, Category = "FPS|Input")
	TObjectPtr<UInputAction> AimWeaponAction;
};

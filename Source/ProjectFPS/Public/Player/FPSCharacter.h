// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class UInputAction;
class AWeapon;

UCLASS()
class PROJECTFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	// ---- Weapon -----
	bool PickupWeapon(AWeapon* Weapon);

	/** 슬롯 번호(1부터)의 무기로 교체. 비어 있으면 무시 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipSlot(int32 SlotNumber);

	UFUNCTION(BlueprintPure, Category = "Weapon")
	AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	// Weapon slots
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<UInputAction>> EquipSlotActions;

	// ---- Components ----
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USkeletalMeshComponent> FirstPersonArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FirstPerson")
	float FirstPersonFOV = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|FirstPerson", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float FirstPersonScale = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraHeightInterpSpeed = 12.f;

	// ---- Weapon 설정 ----
	/** 새 무기를 습득하면 바로 그 무기로 교체할지 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bAutoEquipOnPickup = true;

	/** (테스트용) 시작 시 스폰해서 바로 습득할 무기들. 기본은 비어 있음 = 맨손 시작 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<AWeapon>> StartingWeaponClasses;

	/** 슬롯 번호 -> 습득한 무기 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TMap<int32, TObjectPtr<AWeapon>> WeaponSlots;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeapon> CurrentWeapon;

	// ---- Input Handlers ----
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleCrouch();

private:
	float CurrentCameraZ = 0.f;
};

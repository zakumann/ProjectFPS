// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> CrouchAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Input functions
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();

	// Sprint
	void StartSprint();
	void StopSprint();

	// Crouch
	void StartCrouch();
	void StopCrouch();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crouch")
	FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crouch")
	float CrouchSpeed;
};

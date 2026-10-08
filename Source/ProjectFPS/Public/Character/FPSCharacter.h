// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USkeletalMeshComponent;

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

	// ====== Enhanced Input ======
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

	// ==== Components ====
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

		/** FirstPerson Arm Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonArmMesh;

	// ==== Mesh Offset ====
	//** Camera Arm MeshOffset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Offset")
	FVector ArmMeshRelativeLocation = FVector(0.0f, 0.0f, -160.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Offset")
	FRotator ArmMeshRelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// ==== Movement ====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.0f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Crouch")
	float CrouchTransitionSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Crouch")
	float DefaultCameraZ = 64.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Crouch")
	float CrouchedCameraZ = 30.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Input functions
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);

	// Sprint
	void StartSprint();
	void StopSprint();

	// Crouch
	void StartCrouch();
	void StopCrouch();

	void TryUnCrouch();

private:
	float TargetCameraZ;

	// Sprint state
	bool bIsSprinting = false;

	// Crouch state
	bool bWantsToSprint = false;
	bool bWantsToCrouch = false;
};

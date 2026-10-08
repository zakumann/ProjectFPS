// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ==== Movement Settings ====
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 30.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	// ==== Mesh Visibility ====
	GetMesh()->SetOwnerNoSee(true);

	// ==== First Person Camera ====
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	TargetCameraZ = DefaultCameraZ;
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, DefaultCameraZ));

	// ==== First Person Arm Mesh ====
	FirstPersonArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonArmMesh");
	FirstPersonArmMesh->SetupAttachment(FirstPersonCamera);

	// Camera rotation and location settings
	FirstPersonArmMesh->SetRelativeLocation(ArmMeshRelativeLocation);
	FirstPersonArmMesh->SetRelativeRotation(ArmMeshRelativeRotation);

	// First Person Arm Mesh settings
	FirstPersonArmMesh->SetOnlyOwnerSee(true);
	FirstPersonArmMesh->bCastDynamicShadow = false;
	FirstPersonArmMesh->CastShadow = false;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bWantsToCrouch && bIsCrouched)
	{
		TryUnCrouch();
	}

	const FVector CurrentLocation = FirstPersonCamera->GetRelativeLocation();

	if (!FMath::IsNearlyEqual(CurrentLocation.Z, TargetCameraZ, 0.01f))
	{
		const float NewCrouchZ = FMath::FInterpTo(CurrentLocation.Z, TargetCameraZ, DeltaTime, CrouchTransitionSpeed);

		FirstPersonCamera->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewCrouchZ));
	}
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AFPSCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopSprint);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFPSCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopCrouch);
	}
}

void AFPSCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	// Move the direction
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add movement input
	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AFPSCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	// Add Controller yaw and pitch
	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void AFPSCharacter::StartSprint()
{
	bWantsToSprint = true;

	// Can't sprint if crouching
	if (bIsCrouched)
	{
		return;
	}

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AFPSCharacter::StopSprint()
{
	bWantsToSprint = false;
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFPSCharacter::StartCrouch()
{
	bWantsToCrouch = true;
	if (!bIsCrouched)
	{
		const float UnCrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		Crouch();

		const float HeightDiff = UnCrouchedHalfHeight - GetCharacterMovement()->CrouchedHalfHeight;
		const FVector CurrentCamLoc = FirstPersonCamera->GetRelativeLocation();
		FirstPersonCamera->SetRelativeLocation(FVector(CurrentCamLoc.X, CurrentCamLoc.Y, CurrentCamLoc.Z + HeightDiff));

		TargetCameraZ = CrouchedCameraZ;

		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AFPSCharacter::StopCrouch()
{
	bWantsToCrouch = false;
	TryUnCrouch();
}

void AFPSCharacter::TryUnCrouch()
{
	if (!bIsCrouched)
	{
		return;
	}
	const float CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	UnCrouch();

	if (!bIsCrouched)
	{
		const float UncrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		const float HeightDiff = UncrouchedHalfHeight - CrouchedHalfHeight;

		// Adjust the camera position to match the new capsule height
		const FVector CurrentCamLoc = FirstPersonCamera->GetRelativeLocation();
		FirstPersonCamera->SetRelativeLocation(FVector(CurrentCamLoc.X, CurrentCamLoc.Y, CurrentCamLoc.Z - HeightDiff));

		TargetCameraZ = DefaultCameraZ;

		if (bWantsToSprint)
		{
			StartSprint();
		}
	}
}

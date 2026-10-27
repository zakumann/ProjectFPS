// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	// First Person Rendering: Blocking Clipping
	FirstPersonCamera->bEnableFirstPersonFieldOfView = true;
	FirstPersonCamera->FirstPersonFieldOfView = FirstPersonFOV;
	FirstPersonCamera->bEnableFirstPersonScale = true;
	FirstPersonCamera->FirstPersonScale = FirstPersonScale;

	FirstPersonArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArmMesh"));
	FirstPersonArmMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonArmMesh->SetOnlyOwnerSee(true);
	FirstPersonArmMesh->SetCollisionProfileName(TEXT("NoCollision"));
	FirstPersonArmMesh->SetGenerateOverlapEvents(false);
	FirstPersonArmMesh->SetCastShadow(false);
	FirstPersonArmMesh->bCastDynamicShadow = false;
	FirstPersonArmMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);

	GetMesh()->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);

	CurrentCameraZ = BaseEyeHeight;
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, CurrentCameraZ));

	// Allow Crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Can fall from ledge while crouching
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth Camera height for crouch
	const float TargetZ = bIsCrouched ? CrouchedEyeHeight : BaseEyeHeight;
	CurrentCameraZ = FMath::FInterpTo(CurrentCameraZ, TargetZ, DeltaTime, CameraHeightInterpSpeed);
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, CurrentCameraZ));
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Mapping Contexts are already = FPSPlayerController.h and cpp
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFPSCharacter::ToggleCrouch);
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();

	// X = Right, Y = Forward
	AddMovementInput(GetActorForwardVector(), Axis.Y);
	AddMovementInput(GetActorRightVector(), Axis.X);
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();

	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AFPSCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AFPSCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CurrentCameraZ += HalfHeightAdjust;
}
void AFPSCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CurrentCameraZ -= HalfHeightAdjust;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponBase.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, DefaultEyeHeight));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;	// Mouse rotation

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonArmsMesh");
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->SetOnlyOwnerSee(true);           // 본인(플레이어) 화면에만 렌더링
	Mesh1P->bCastDynamicShadow = false;      // 1인칭 팔의 어색한 그림자 제거
	Mesh1P->CastShadow = false;
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 벽 뚫림/밀림 방지
	// 카메라 기준 기본 위치/회전 오프셋 (에디터 뷰포트에서 사용하는 팔 에셋에 맞춰 미세 조정)
	Mesh1P->SetRelativeLocation(FVector(0.f, 0.f, -155.f));
	Mesh1P->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true; // Can fall ledge even doing Crouching
	GetCharacterMovement()->SetCrouchedHalfHeight(48.f);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// EnhancedInput -> Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (FirstPersonMappingContext)
			{
				Subsystem->AddMappingContext(FirstPersonMappingContext, 0);
			}
		}
	}

	if (DefaultWeaponClass && Mesh1P)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 무기 액터 월드에 스폰 후 팔 메시 소켓에 장착
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->EquipToCharacter(Mesh1P);
		}
	}

}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Mesh1P)
	{
		const float TargetZ = bIsCrouched ? CrouchEyeHeight : DefaultEyeHeight;
		FVector CurrentLocation = FirstPersonCameraComponent->GetRelativeLocation();

		if (!FMath::IsNearlyEqual(CurrentLocation.Z, TargetZ, 0.01f))
		{
			CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, TargetZ, DeltaTime, CrouchInterpSpeed);
			FirstPersonCameraComponent->SetRelativeLocation(CurrentLocation);
		}
	}
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		// Look
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		// Jump
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Sprint
		EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AFPSCharacter::StartSprint);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopSprint);
		// Crouch
		EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFPSCharacter::ToggleCrouch);
		// Fire
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &AFPSCharacter::FireWeapon);
	}
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFPSCharacter::Look(const FInputActionValue & Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSCharacter::StartSprint()
{
	if (bIsCrouched) return;

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void AFPSCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}
void AFPSCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		StopSprint();
		Crouch();
	}
	
}

// 클래스 하단에 함수 구현 추가
void AFPSCharacter::FireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

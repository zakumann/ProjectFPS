// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Combat/CombatComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(SpringArm);
	FirstPersonCamera->bUsePawnControlRotation = false;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh1P");
	Mesh1P->SetupAttachment(FirstPersonCamera);
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
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

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(CycleWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_CycleWeapon);
	EIC->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_FireWeapon_Pressed);
	EIC->BindAction(FireWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_FireWeapon_Released);
	EIC->BindAction(AimWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_Aim_Pressed);
	EIC->BindAction(AimWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_Aim_Released);
	EIC->BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_ReloadWeapon);
}

void AFPSCharacter::Input_CycleWeapon()
{
	Combat->Initiate_CycleWeapon();
}

void AFPSCharacter::Input_ReloadWeapon()
{
	Combat->Initiate_ReloadWeapon();
}

void AFPSCharacter::Input_FireWeapon_Pressed()
{
	Combat->Initiate_FireWeapon_Pressed();
}

void AFPSCharacter::Input_FireWeapon_Released()
{
	Combat->Initiate_FireWeapon_Released();
}

void AFPSCharacter::Input_Aim_Pressed()
{
	Combat->Initiate_Aim_Pressed();
}

void AFPSCharacter::Input_Aim_Released()
{
	Combat->Initiate_Aim_Released();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FPSCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	SetRootComponent(PickupSphere);
	PickupSphere->InitSphereRadius(80.f);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionObjectType(ECC_WorldDynamic);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickupSphere->SetGenerateOverlapEvents(true);

	// 무기 메시 (월드 상태에서는 보이기만 하고 충돌 없음)
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(PickupSphere);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnPickupSphereBeginOverlap);
}

void AWeapon::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsPickedUp)
	{
		return;
	}

	if (AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
	{
		Character->PickupWeapon(this);
	}
}

void AWeapon::OnPickedUp(AActor* NewOwner, USkeletalMeshComponent* ArmMesh)
{
	if (bIsPickedUp || !ArmMesh)
	{
		return;
	}
	bIsPickedUp = true;

	SetOwner(NewOwner);

	// 더 이상 습득 판정/충돌 없음
	PickupSphere->OnComponentBeginOverlap.RemoveAll(this);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetGenerateOverlapEvents(false);

	// 1인칭 렌더링: 본인에게만 보이고, 벽 클리핑 방지(First Person Rendering) 적용
	WeaponMesh->SetOnlyOwnerSee(true);
	WeaponMesh->SetCastShadow(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);

	// ArmMesh의 무기별 소켓에 부착 (없으면 hand_r 폴백)
	FName SocketToUse = HandSocketName;
	if (!ArmMesh->DoesSocketExist(SocketToUse))
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon: '%s' 소켓이 ArmMesh에 없어 hand_r로 폴백합니다."), *SocketToUse.ToString());
		SocketToUse = TEXT("hand_r");
	}
	AttachToComponent(ArmMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketToUse);
}

void AWeapon::SetEquipped(bool bEquipped)
{
	bIsEquipped = bEquipped;
	SetActorHiddenInGame(!bEquipped);
}


// Called every frame
/*void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


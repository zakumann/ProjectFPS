// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// 손에 들린 무기가 캐릭터 캡슐과 충돌해 밀어내는 현상 방지
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCastShadow(false);
}

void AWeaponBase::EquipToCharacter(USkeletalMeshComponent* TargetMesh)
{
	if (!TargetMesh) return;

	// 지정된 소켓(AttachSocketName)에 위치/회전/크기를 맞춰 부착
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(TargetMesh, AttachRules, AttachSocketName);
}

void AWeaponBase::Fire()
{

}

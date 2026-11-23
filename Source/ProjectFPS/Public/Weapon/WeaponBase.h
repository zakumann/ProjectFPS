// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class PROJECTFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	/** 무기를 캐릭터의 특정 스켈레탈 메시 소켓에 장착하는 함수 */
	virtual void EquipToCharacter(USkeletalMeshComponent* TargetMesh);

	/** 자식 무기(권총, 라이플 등)에서 재정의할 사격 함수 */
	virtual void Fire();

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE FName GetAttachSocketName() const { return AttachSocketName; }

protected:
	/** 무기 3D 모델 (총구 소켓, 탄창 본 등 활용을 위해 SkeletalMesh 사용) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/** 장착될 팔 스켈레톤의 소켓 이름 (기본값: hand_r 소켓) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName AttachSocketName = FName("WeaponSocket_R");
};

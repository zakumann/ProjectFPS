// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UPrimitiveComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Dagger,
	Pistol,
	AssaultRifle,
	Shotgun
};

UCLASS()
class PROJECTFPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	/** 플레이어가 습득했을 때 호출: ArmMesh의 HandSocketName 소켓에 부착하고 1인칭 렌더링으로 전환 */
	void OnPickedUp(AActor* NewOwner, USkeletalMeshComponent* ArmMesh);

	/** 장착(보임) / 비장착(숨김) 전환 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetEquipped(bool bEquipped);

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsPickedUp() const { return bIsPickedUp; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsEquipped() const { return bIsEquipped; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	EWeaponType GetWeaponType() const { return WeaponType; }

	/** 1부터 시작하는 슬롯 번호 (1 Dagger, 2 Pistol, 3 AssaultRifle, 4 Shotgun ...) */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetWeaponSlot() const { return WeaponSlot; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FName GetHandSocketName() const { return HandSocketName; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USphereComponent> PickupSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Dagger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "1"))
	int32 WeaponSlot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FText WeaponName;

	/** ArmMesh(SK_Mannequin_Arms)의 hand_r 하위에 만든 소켓 이름 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName HandSocketName = TEXT("hand_r");

private:
	bool bIsPickedUp = false;
	bool bIsEquipped = false;

public:	
	// Called every frame
	/*virtual void Tick(float DeltaTime) override;*/
};

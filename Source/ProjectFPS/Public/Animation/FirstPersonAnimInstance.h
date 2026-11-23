// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FirstPersonAnimInstance.generated.h"

class AFPSCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTFPS_API UFirstPersonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	/** 소유 캐릭터 및 무브먼트 캐싱 */
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<AFPSCharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	/** 애니메이션 그래프에서 사용할 상태 변수들 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.f; // 수평 이동 속도 (0 ~ 800)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bShouldMove = false; // 실제로 이동 입력이 있고 움직이는 중인지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsFalling = false; // 점프 또는 낙하 중인지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false; // 달리기 상태인지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouched = false; // 앉기 상태인지
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/FirstPersonAnimInstance.h"

#include "Character/FPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFirstPersonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cast Owning Pawn from AFPSCharacter
	OwnerCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UFirstPersonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 에디터 프리뷰나 초기 프레임에서 참조가 비어있을 경우 재시도
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());
		if (OwnerCharacter)
		{
			MovementComponent = OwnerCharacter->GetCharacterMovement();
		}
	}

	if (!OwnerCharacter || !MovementComponent) return;

	// 1. 수평 이동 속도 (Z축 점프/낙하 속도 제외)
	FVector Velocity = OwnerCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	// 2. 가속도 입력이 존재하고 속도가 일정 이상일 때 이동 중으로 판정
	bShouldMove = (GroundSpeed > 3.f) && (MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

	// 3. 공중(점프/낙하) 및 앉기 상태 동기화
	bIsFalling = MovementComponent->IsFalling();
	bIsCrouched = OwnerCharacter->bIsCrouched;

	// 4. 달리기 상태 판정 (캐릭터에서 Getter 함수를 제공받아 확인)
	bIsSprinting = OwnerCharacter->IsSprinting();
}
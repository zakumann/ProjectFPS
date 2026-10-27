// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSController/FPSPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// There is only subsystem in Local Player Controller
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, MappingContextPriority);
		}
	}
}

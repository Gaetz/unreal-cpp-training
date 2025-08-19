// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacterController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Player/MainCharacter.h"

void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// Clear out existing mapping and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	// Bind movement inputs
	auto EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInput->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainCharacterController::MovePlayer);
	EnhancedInput->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AMainCharacterController::Look);
	EnhancedInput->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainCharacterController::Jump);
}

void AMainCharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	Character = Cast<AMainCharacter>(InPawn);
}

void AMainCharacterController::MovePlayer(const FInputActionValue& Value)
{
	if (!Character) return;

	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (MoveValue.Y != 0.f)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveValue.Y);
	}
	if (MoveValue.X != 0.f)
	{
		Character->AddMovementInput(Character->GetActorRightVector(), MoveValue.X);
	}
}

void AMainCharacterController::Look(const FInputActionValue& Value)
{
	if (!Character) return;

	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.X != 0.f)
	{
		Character->AddControllerYawInput(LookValue.X);
	}
	if (LookValue.Y != 0.f)
	{
		Character->AddControllerPitchInput(-LookValue.Y);
	}

}

void AMainCharacterController::Jump()
{
	if (!Character) return;
	Character->Jump();
}

void AMainCharacterController::AddPitchInput(float Val)
{
	float Multiplier = Val * MouseSensitivityY;
	Super::AddPitchInput(Multiplier);
}

void AMainCharacterController::AddYawInput(float Val)
{
	float Multiplier = Val * MouseSensitivityX;
	Super::AddYawInput(Multiplier);
}

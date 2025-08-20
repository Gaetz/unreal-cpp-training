// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Player/MainCharacter.h"
#include "Gameplay/GravityGunComponent.h"

// Sets default values for this component's properties
UGravityGunController::UGravityGunController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UGravityGunController::SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, AMainCharacter* InPawn)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;
	EnhancedInputComponent->BindAction(InputActionTakeObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnTakeObjectInputPressed);
	EnhancedInputComponent->BindAction(InputActionThrowObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowObjectInputTriggered);
	EnhancedInputComponent->BindAction(InputActionThrowForceMultiplier, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowForceMultiplierInputPressed);

	Character = InPawn;
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();
}

void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
	
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Non-const is useful pour the input binding
void UGravityGunController::OnTakeObjectInputPressed()
{
	if (GravityGunComponent.IsValid())
	{
		GravityGunComponent->OnTakeObjectInputPressed();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Non-const is useful pour the input binding
void UGravityGunController::OnThrowObjectInputTriggered(const FInputActionValue& Value)
{
	if (!GravityGunComponent.IsValid()) return;
	const float CountValue = Value.Get<float>();
	if (CountValue > 0.f)
	{
		GravityGunComponent->OnThrowObjectInputPressed();
	}
	else
	{
		GravityGunComponent->OnThrowObjectInputReleased();
	}
}

void UGravityGunController::OnThrowForceMultiplierInputPressed()
{
	if (!GravityGunComponent.IsValid()) return;
	GravityGunComponent->OnThrowForceMultiplierInputPressed();
}


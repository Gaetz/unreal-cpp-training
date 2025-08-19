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

	// ...
}

void UGravityGunController::SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, AMainCharacter* InPawn)
{
	Character = InPawn;
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();

	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;
	EnhancedInputComponent->BindAction(InputActionTakeObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnTakeObjectInputPressed);
	EnhancedInputComponent->BindAction(InputActionThrowObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowObjectInputPressed);
}


// Called when the game starts
void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGravityGunController::OnTakeObjectInputPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Take object"));
}

void UGravityGunController::OnThrowObjectInputPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Throw object"));
}



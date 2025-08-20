// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PickUpComponent.h"

// Sets default values for this component's properties
UPickUpComponent::UPickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPickUpComponent::StartDestroyTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(DestroyTimerHandle);
	TimerManager.SetTimer(DestroyTimerHandle, this, &UPickUpComponent::DestroyPickUp, PickUpStruct.DestroyTime, false);
}

void UPickUpComponent::DestroyPickUp()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(DestroyTimerHandle);

	OnPickUpDestroyed.Broadcast();
	GetOwner()->Destroy();
}

EPickUpType UPickUpComponent::GetPickUpType() const
{
	return PickUpStruct.PickUpType;
}

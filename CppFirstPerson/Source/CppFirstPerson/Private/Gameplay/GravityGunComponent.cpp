// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Player/MainCharacter.h"
#include "Objects/PickUpComponent.h"

// Sets default values for this component's properties
UGravityGunComponent::UGravityGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMainCharacter>(GetOwner());
	CharacterCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(GravityGunCollisionTraceChannel);


}


// Called every frame
void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	// Launch a ray to find a pick up object
	FVector RaycastStart = CharacterCameraManager->GetCameraLocation();
	FVector RaycastEnd = RaycastStart + CharacterCameraManager->GetActorForwardVector() * RaycastSize;

	// Raycast
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character.Get());
	FHitResult RaycastHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, GravityGunCollisionChannel, Params);
	if (!bHit)
	{
		return;
	}

	// Debug display raycast
#if !UE_BUILD_SHIPPING
	if (bDrawDebugRaycast)
	{
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 3.5f);
	}
#endif

	// Get pick up
	CurrentPickUp = RaycastHit.GetActor();
	if (!CurrentPickUp) return;
	CurrentPickUpComponent = CurrentPickUp->GetComponentByClass<UPickUpComponent>();
	if (!CurrentPickUpComponent) return;
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *CurrentPickUp->GetName());
}
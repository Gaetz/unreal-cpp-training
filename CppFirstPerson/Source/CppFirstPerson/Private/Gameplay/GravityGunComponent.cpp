// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunComponent.h"

#include "MathUtil.h"
#include "Kismet/GameplayStatics.h"

#include "Player/MainCharacter.h"
#include "Objects/PickUpComponent.h"

// Sets default values for this component's properties
UGravityGunComponent::UGravityGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMainCharacter>(GetOwner());
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(GravityGunCollisionTraceChannel);
}


// Called every frame
void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePickUpLocation();
	UpdateThrowForceTimer(DeltaTime);
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	// Check we do not already have a pickup object
	if (CurrentPickUp.IsValid())
	{
		ReleasePickUp();
		return;
	}
	
	// Launch a ray to find a pickup object
	const FVector RaycastStart = CameraManager->GetCameraLocation();
	const FVector RaycastEnd = RaycastStart + CameraManager->GetActorForwardVector() * RaycastSize;

	// Debug display raycast
#if !UE_BUILD_SHIPPING
	if (bDrawDebugRaycast)
	{
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 3.5f);
	}
#endif

	// Raycast
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character.Get());
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RaycastStart, RaycastEnd, GravityGunCollisionChannel, Params);
	if (!bHit)
	{
		return;
	}

	// Get pick up
	CurrentPickUp = HitResult.GetActor();

	CurrentPickUpComponent = CurrentPickUp->GetComponentByClass<UPickUpComponent>();
	if (!CurrentPickUpComponent.IsValid()) return;
	CurrentPickUpMesh = CurrentPickUp->GetComponentByClass<UStaticMeshComponent>();
	if (!CurrentPickUpMesh.IsValid()) return;

	// Update pick up collision profile and physics
	CurrentPickUpMesh->SetSimulatePhysics(false);
	PickUpPreviousCollisionProfileName = CurrentPickUpMesh->GetCollisionProfileName();
	CurrentPickUpMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *CurrentPickUp->GetName());

	switch (CurrentPickUpComponent->GetPickUpType())
	{
	case EPickUpType::DestroyAfterThrow:
		// Reset timer if picked up again
		CurrentPickUpComponent->ClearDestroyTimer();
		break;
	case EPickUpType::DestroyAfterPickup:
		// Reset timer if picked up again
		CurrentPickUpComponent->ClearDestroyTimer();
		// Start destroy timer and trigger pickup holding management
		CurrentPickUpComponent->StartDestroyTimer();
		CurrentPickUpComponent->OnPickUpDestroyed.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
		break;
	default:
		break;
	}
}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	if (!CurrentPickUp.IsValid()) return;
	// Prepare pickup throw
	bUpdateThrowForceTimer = true;
	CurrentTimeToReachMaxThrowForce = 0.f;
}

void UGravityGunComponent::OnThrowObjectInputReleased()
{
	if (!CurrentPickUp.IsValid()) return;

	switch (CurrentPickUpComponent->GetPickUpType())
	{
	case EPickUpType::DestroyAfterThrow:
		// Start destroy timer
		CurrentPickUpComponent->StartDestroyTimer();
		break;
	default:
		break;
	}

	ReleasePickUp(true);
	// Reset throw force timer
	bUpdateThrowForceTimer = false;
	CurrentTimeToReachMaxThrowForce = 0.f;
}

void UGravityGunComponent::UpdatePickUpLocation() const
{
	if (!CurrentPickUp.IsValid()) return;
	const FVector NewLocation = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * PickUpDistanceFromPlayer;
	const FRotator NewRotation = CameraManager->GetCameraRotation();
	CurrentPickUp->SetActorLocationAndRotation(NewLocation, NewRotation);
}

void UGravityGunComponent::ReleasePickUp(const bool bThrow)
{
	if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterPickup)
	{
		// Remove the delegate
		CurrentPickUpComponent->OnPickUpDestroyed.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}
	
	// Set back physics and collision profile
	CurrentPickUpMesh->SetSimulatePhysics(true);
	CurrentPickUpMesh->SetCollisionProfileName(PickUpPreviousCollisionProfileName);

	if (bThrow)
	{
		float ThrowForceAlpha;
		float ThrowForce;
		ThrowForceAlpha = FMath::Clamp(CurrentTimeToReachMaxThrowForce / TimeToReachMaxThrowForce, 0.f, 1.f);
		ThrowForce = FMath::Lerp(PickUpThrowForce, PickUpMaxThrowForce, ThrowForceAlpha) * CurrentPickUpThrowForceMultiplier;
	
		const FVector Impulse = CameraManager->GetActorForwardVector() * ThrowForce;
		CurrentPickUpMesh->AddImpulse(Impulse);
		const FVector AngularImpulse = FVector(
			FMath::RandRange(0.0, PickUpThrowAngularForce.X),
			FMath::RandRange(0.0, PickUpThrowAngularForce.Y),
			FMath::RandRange(0.0, PickUpThrowAngularForce.Z)
		);
		CurrentPickUpMesh->AddAngularImpulseInDegrees(AngularImpulse * CurrentPickUpThrowForceMultiplier);
	}
	
	// Clear references
	CurrentPickUp = nullptr;
	CurrentPickUpComponent = nullptr;
	CurrentPickUpMesh = nullptr;
}

void UGravityGunComponent::UpdateThrowForceTimer(float DeltaTime)
{
	if (!bUpdateThrowForceTimer) return;
	CurrentTimeToReachMaxThrowForce += DeltaTime;

	/*
	// Predict projectile path
	FPredictProjectilePathParams PredictParams;
	FPredictProjectilePathResult PredictResult;
	PredictParams.StartLocation = CurrentPickUp->GetActorLocation();
	PredictParams.LaunchVelocity = CameraManager->GetActorForwardVector() * PickUpThrowForce;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 0.f;
	PredictParams.MaxSimTime = 2.f;
	PredictParams.bTraceWithChannel = true;
	PredictParams.TraceChannel = ECollisionChannel::ECC_WorldStatic;
	PredictParams.ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	PredictParams.SimFrequency = 25.f;
	PredictParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	PredictParams.DrawDebugTime = 5.f;
	PredictParams.bTraceComplex = false;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);
	*/
}

void UGravityGunComponent::OnHoldPickUpDestroyed()
{
	CurrentPickUpComponent->OnPickUpDestroyed.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	ReleasePickUp();
}

void UGravityGunComponent::OnThrowForceMultiplierInputPressed()
{
	CurrentPickUpThrowForceMultiplier = CurrentPickUpThrowForceMultiplier == 1.f ? PickUpThrowForceMultiplier : 1.f;
}

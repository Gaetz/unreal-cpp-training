// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(GravityGun), meta=(BlueprintSpawnableComponent) )
class CPPFIRSTPERSON_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed();
	void OnThrowObjectInputReleased();


// Debug
protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Debug")
	bool bDrawDebugRaycast = false;

	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Debug", meta = (EditCondition = "bDrawDebugRaycast"))
	float TimerDebugRaycast = 5.f;


// Collisions and raycast
protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Collision", meta = (ClampMin = "0.0", ClampMax = "5000.0"))
	float RaycastSize = 500.f;

	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;

	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;


// Pick Up
protected:
	TWeakObjectPtr<AActor> CurrentPickUp = nullptr;
	TWeakObjectPtr<class UPickUpComponent> CurrentPickUpComponent = nullptr;
	TWeakObjectPtr<class UStaticMeshComponent> CurrentPickUpMesh = nullptr;
	FName PickUpPreviousCollisionProfileName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float PickUpDistanceFromPlayer = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "-200.0", ClampMax = "200.0"))
	float PickUpHeightFromPlayer = -10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float PickUpThrowForce = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up")
	FVector PickUpThrowAngularForce = FVector(50000.f, 30000.f, 60000.f);

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float PickUpMaxThrowForce = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float TimeToReachMaxThrowForce = 3.f;
	float CurrentTimeToReachMaxThrowForce = 0.f;
	
	bool bUpdateThrowForceTimer = false;

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float PickUpThrowForceMultiplier = 3.f;
	float CurrentPickUpThrowForceMultiplier = 1.f;
	
	void UpdatePickUpLocation() const;
	void ReleasePickUp(bool bThrow = false);
	void UpdateThrowForceTimer(float DeltaTime);

public:
	void OnThrowForceMultiplierInputPressed();
};

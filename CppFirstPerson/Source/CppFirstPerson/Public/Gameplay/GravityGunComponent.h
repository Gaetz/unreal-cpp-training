// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickUpTakenDelegate, int, NumberPickUpTaken);

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

	UPROPERTY(EditAnywhere, Category = "Gravity Gun")
	class UGravityGunDataAsset* GravityGunDataAsset = nullptr;
	
	float CurrentTimeToReachMaxThrowForce = 0.f;
	bool bUpdateThrowForceTimer = false;

	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Pick Up", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float PickUpThrowForceMultiplier = 3.f;
	float CurrentPickUpThrowForceMultiplier = 1.f;
	
	void UpdatePickUpLocation() const;
	void ReleasePickUp(bool bThrow = false);
	void UpdateThrowForceTimer(float DeltaTime);
	void OnUpdateGravityGunDataAsset();
	

	UFUNCTION()
	void OnHoldPickUpDestroyed();

public:
	void OnThrowForceMultiplierInputPressed();

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

// Counting pick ups
protected:
	int PickUpTaken = 0;

public:
	void TakePickUp(class APickUpComponent* PickUp);
	bool IsHoldingPickUp();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPickUpTakenDelegate OnPickUpTakenDelegate;

	
// Throw for progress bar
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gravity Gun")
	float GetTimeToReachMaxThrowForce() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gravity Gun")
	float GetCurrentTimeToReachMaxThrowForce() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Curve")
	class UCurveFloat* ThrowForceCurve = nullptr;
};

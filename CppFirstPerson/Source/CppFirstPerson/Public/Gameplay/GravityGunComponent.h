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


// Debug
protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Debug")
	bool bDrawDebugRaycast = false;

	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Debug", meta = (EditCondition = "bDrawDebugRaycast"))
	float TimerDebugRaycast = 5.f;


// Collisions and raycast
protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Collision", meta = (ClampMin = "0.0", ClampMax = "5000.0"))
	float RaycastSize = 50.f;

	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;

	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CharacterCameraManager = nullptr;


// Pick Up
protected:
	class AActor* CurrentPickUp = nullptr;
	class UPickUpComponent* CurrentPickUpComponent = nullptr;
};

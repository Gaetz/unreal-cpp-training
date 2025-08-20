// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickUpDestroyDelegate);

// Enum to define the type of pick up behavior
UENUM()
enum class EPickUpType : uint8
{
	Normal,
	DestroyAfterPickup,
	DestroyAfterThrow,
	Max UMETA(Hidden)
};


// Pick up type data
USTRUCT(BlueprintType)
struct FPickUpStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPickUpType PickUpType = EPickUpType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PickUpType != EPickUpType::Normal", EditConditionHides))
	float DestroyTime = 5.f;
};


// Pick Up Component
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPFIRSTPERSON_API UPickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickUpComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	FPickUpStruct PickUpStruct {};

	
// Destruction
protected:
	FTimerHandle DestroyTimerHandle;
	
	void DestroyPickUp();

public:
	FOnPickUpDestroyDelegate OnPickUpDestroyed;
	
	void StartDestroyTimer();
	void ClearDestroyTimer();
	EPickUpType GetPickUpType() const;
};

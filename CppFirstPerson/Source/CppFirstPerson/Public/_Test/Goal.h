// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UENUM()
enum class EGoalTeam : uint8
{
	None UMETA(Hidden),
	Red,
	Blue,
	MAX UMETA(Hidden),
};

UCLASS()
class CPPFIRSTPERSON_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPointLightComponent* PointLight = nullptr;

	unsigned int Score = 0;
	EGoalTeam GoalTeam = EGoalTeam::Red;

	UFUNCTION()
	void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void UpdatePointLight();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
};

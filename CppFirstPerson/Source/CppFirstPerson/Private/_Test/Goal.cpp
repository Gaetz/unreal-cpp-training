// Fill out your copyright notice in the Description page of Project Settings.


#include "_Test/Goal.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

#include "Objects/PickUpComponent.h"

AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("CollisionBox"));
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}
	PointLight = ObjectInitializer.CreateOptionalDefaultSubobject<UPointLightComponent>(this, TEXT("PointLight"));
	if (PointLight)
	{
		PointLight->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnGoalOverlap);
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPickUpComponent* EnteringPickUpComponent = OtherActor->FindComponentByClass<UPickUpComponent>();
	if (!EnteringPickUpComponent) return;

	Score++;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void AGoal::UpdatePointLight()
{
	if (!PointLight) return;
	// Set light color based on team
	FLinearColor LightColor = FLinearColor::White;
	switch (GoalTeam)
	{
	case EGoalTeam::Red:
		LightColor = FLinearColor::Red;
		break;
	case EGoalTeam::Blue:
		LightColor = FLinearColor::Blue;
		break;
	default:
		break;
	}
	PointLight->SetLightColor(LightColor);
}

void AGoal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if (IsTemplate() || !HasAnyFlags(RF_Transient | RF_NeedLoad)) return;
	UpdatePointLight();
#endif
	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "_Test/Goal.h"
#include "Components/BoxComponent.h"
#include "Objects/PickUpComponent.h"

AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("CollisionBox"));
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
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


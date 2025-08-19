// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunController.generated.h"


UCLASS( Abstract, Blueprintable, ClassGroup=(GravityGun), meta=(BlueprintSpawnableComponent) )
class CPPFIRSTPERSON_API UGravityGunController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityGunController();

	void SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, class AMainCharacter* InPawn);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class UGravityGunComponent> GravityGunComponent = nullptr;

		
// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionTakeObject;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionThrowObject;

	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed(const struct FInputActionValue& Value);
};

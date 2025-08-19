// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class CPPFIRSTPERSON_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;


// Character
protected:
	class AMainCharacter* Character = nullptr;
	TWeakObjectPtr<class UGravityGunController> GravityGunController = nullptr;



// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionMove;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Mouse")
	class UInputAction* InputActionLook;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Jump")
	class UInputAction* InputActionJump;

	
protected:
	void MovePlayer(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Jump();



// Mouse sensitivity
protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input|Mouse")
	float MouseSensitivityX = 1.f;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input|Mouse")
	float MouseSensitivityY = 1.f;

public:
	virtual void AddPitchInput(float Val) override;
	virtual void AddYawInput(float Val) override;

};

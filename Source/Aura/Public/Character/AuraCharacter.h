// Copyright Cuss Programming

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:

	AAuraCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComponent; //allows third person camera arm

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComponent;
	
};

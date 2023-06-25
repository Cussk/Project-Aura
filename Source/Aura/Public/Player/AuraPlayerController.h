// Copyright Cuss Programming

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
struct FInputActionValue;


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	bool bShiftKeyDown = false;

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	FHitResult CursorHit;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	/* CLICK TO MOVE */
	FVector CachedDestination = FVector::ZeroVector;

	float FollowTime = 0.0f;

	float ShortPressThreshold = 0.5f;

	bool bAutoRunning = false;

	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly, Category = "ClickToMove")
	float AutoRunAcceptanceRadius = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "ClickToMove")
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};

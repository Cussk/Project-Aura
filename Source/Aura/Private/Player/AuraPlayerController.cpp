// Copyright Cuss Programming


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;

	//if actor under cursor has IEnemyInterface will get that actor, otherwise will be null
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 *Line case from cursor. Multiple results
	 * A. LastActor null && THisActor null
	 *		-Do Nothing.
	 * B. LastACtor null && ThisActor valid
	 *		-Highlight ThisActor
	 * C. LastActorValid && ThisActor null
	 *		-Unhighlight LastActor
	 * D. Both Actors valid, but LastActor != ThisActor
	 *		-Unhighlight LastActor, Highlight ThisActor
	 * E. Both actors valid && LastActor = THisActor
	 *      -Do nothing
	 *
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//Case B
			ThisActor->HighlightActor();
		}
		else
		{
			//Case A - both null, do nothing
		}
	}
	else //LastActor valid
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnhighlightActor();
		}
		else //both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnhighlightActor();
				ThisActor->HighlightActor();
			}
			else 
			{
				//Case E - LastActor = ThisActor, do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check null, crash if null
	check(AuraContext);

	//Gets local players input subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem); //check null

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//allow input to interact with game and ui
	FInputModeGameAndUI InputModeData;
	//allows use of mouse in-game
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//keeps cursor visible in-game
	InputModeData.SetHideCursorDuringCapture(false);
	//sets input modewith above defined variables
	SetInputMode(InputModeData);


}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//cast and check for null inline, will crash if null
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//binds action varibles to action functions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	//left/right rotation
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	//gets normalized direction vectors
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) // check null
	{
		//movement vector, scaled to allow back/forward, left/right
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

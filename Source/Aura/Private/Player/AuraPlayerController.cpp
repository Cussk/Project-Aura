// Copyright Cuss Programming


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>

#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	//check if Pawn is a player pawn
	if (APawn* ControlledPawn = GetPawn())
	{
		//location on spline closest to players position
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		//direction for spline point
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		//move pawn in direction of spline point
		ControlledPawn->AddMovementInput(Direction);

		//length of distance between cursor hit point and spline point location 
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

		//if within acceptance radius distance
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			//stop movement
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;

	//if actor under cursor has IEnemyInterface will get that actor, otherwise will be null
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//highlight actor based on cases
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnhighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}

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
	}*/
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//if input tag is left mouse button
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//if ThisActor is not nullptr true else false
		bTargeting = ThisActor ? true : false;

		bAutoRunning = false;
	}

	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());

	//if input tag is not left mouse button
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent()) //check null
		{
			//use released action ability of chosen input tag
			GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		}

		return;
	}

	if (GetAbilitySystemComponent()) //check null
	{
		//use released action ability of chosen input tag
		GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting || !bShiftKeyDown) //perform autorun movement
	{
		const APawn* ControlledPawn = GetPawn();
		//was click a short press
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			//get navigation path
			if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				//clear previous spline points before loop
				Spline->ClearSplinePoints();

				//loop through location points in the navigation path
				for (const FVector& PointLocation : NavigationPath->PathPoints)
				{
					//add point to spline
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);

					DrawDebugSphere(GetWorld(), PointLocation, 8.0f, 8, FColor::Green, false, 5.0f);
				}

				//set CachedDestination to last point in NavigationPath
				CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];

				bAutoRunning = true;
			}
		}

		//reset movement variables
		FollowTime = 0.0f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//if input tag is not left mouse button
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent()) //check null
		{
			//use the held action ability of chosen input tag
			GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}

		return;
	}

	if (bTargeting || bShiftKeyDown) //use ability
	{
		if (GetAbilitySystemComponent()) //check null
		{
			//use the held action ability of chosen input tag
			GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
	}
	else //perform movement
	{
		//increment follow time with delta seconds as long as LMB is held 
		FollowTime += GetWorld()->GetDeltaSeconds();

		//Gets hit result from location of mouse cursor in the world
		if (CursorHit.bBlockingHit)
		{
			//cache the point where the hit occurs in world
			CachedDestination = CursorHit.ImpactPoint;
		}

		//check if controlled pawn is player pawn
		if (APawn* ControlledPawn = GetPawn())
		{
			//destination for movement, vector between world hit and actors beginning location
			const FVector WorldDestination = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			//move pawn to destination
			ControlledPawn->AddMovementInput(WorldDestination);
		}
	}

	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());

	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	//if null get ASC by cast
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check null, crash if null
	check(AuraContext);

	//Gets local players input subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) //check null
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

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
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	//binds action varibles to action functions
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	//bind to input type function
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

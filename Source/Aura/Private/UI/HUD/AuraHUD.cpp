// Copyright Cuss Programming


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	//if null create new widget controller
	if (OverlayWidgetController == nullptr)
	{
		//create controller
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//initialize variables
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		//binds to delegate for event changes
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	//if null create new widget controller
	if (AttributeMenuWidgetController == nullptr)
	{
		//create controller
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		//initialize variables
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		//binds to delegate for event changes
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
                           UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	//if null crash with described errors
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	//get overlay widget class and add to viewport
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	//controller params struct
	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	//get copy of widget controller
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);


	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();



	Widget->AddToViewport();
}



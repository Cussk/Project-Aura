// Copyright Cuss Programming


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	//get player controller
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) //check null
	{
		//get player HUD
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			//get player state
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			//get player ability system component
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			//get player attribute set
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			//get widget controller params struct
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

			//get overlay widget controller initialized with widget controller params
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	//get player controller
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) //check null
	{
		//get player HUD
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			//get player state
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			//get player ability system component
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			//get player attribute set
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			//get widget controller params struct
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

			//get attribute menu widget controller initialized with widget controller params
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	//get aura game mode base
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameModeBase == nullptr) return;

	//get avatar actor
	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	//get characterclassinfo class
	UCharacterClassInfo* CharacterClassInfo = AuraGameModeBase->CharacterClassInfo;

	//get character class default info struct
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	/* PRIMARY ATTRIBUTES */

	//get gameplay effect context handle and add source to it for use on MMC
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

	//gets Primary Attributes Gameplay Effect from CharacterClassDefaultInfo struct
	const FGameplayEffectSpecHandle PrimaryAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);

	//applies primary attributes to owner of ASC
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesEffectSpecHandle.Data.Get());

	/* SECONDARY ATTRIBUTES */

	//get gameplay effect context handle and add source to it for use on MMC
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	//gets Secondary Attributes Gameplay Effect from CharacterClassInfo class
	const FGameplayEffectSpecHandle SecondaryAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);

	//applies Secondary attributes to owner of ASC
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesEffectSpecHandle.Data.Get());

	/* VITAL ATTRIBUTES */

	//get gameplay effect context handle and add source to it for use on MMC
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);

	//gets Vital Attributes Gameplay Effect from CharacterClassInfo class
	const FGameplayEffectSpecHandle VitalAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);

	//applies Vital attributes to owner of ASC
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesEffectSpecHandle.Data.Get());
}

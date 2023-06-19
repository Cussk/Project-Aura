// Copyright Cuss Programming


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    //bind to delegate
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

    //get gameplay tags singleton
   /* const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    GEngine->AddOnScreenDebugMessage(
        -1,
        10.f,
        FColor::Orange,
        FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString())
    );*/
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    //GEngine->AddOnScreenDebugMessage(1, 80.f, FColor::Blue, FString("Effect Applied!"));

    FGameplayTagContainer TagContainer;

    EffectSpec.GetAllAssetTags(TagContainer);

    //Broadcast TagContainer to anything bound to this delegate
    EffectAssetTags.Broadcast(TagContainer);
}

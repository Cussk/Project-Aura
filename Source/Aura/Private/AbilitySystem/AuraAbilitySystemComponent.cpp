// Copyright Cuss Programming


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    //bind to delegate
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
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

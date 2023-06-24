// Copyright Cuss Programming


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    //bind to delegate
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

    //get gameplay tags singleton
   /* const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    GEngine->AddOnScreenDebugMessage(
        -1,
        10.f,
        FColor::Orange,
        FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString())
    );*/
}

//client rpc, called on server replicated to client
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    //GEngine->AddOnScreenDebugMessage(1, 80.f, FColor::Blue, FString("Effect Applied!"));

    FGameplayTagContainer TagContainer;

    EffectSpec.GetAllAssetTags(TagContainer);

    //Broadcast TagContainer to anything bound to this delegate
    EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        //gets ability information
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.0f);

        //get aura gameplay ability through ability spec
        if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability)) // check null
        {
            //add startup tag to dynamic tag container
            AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);

            //grant ability to actor
            GiveAbility(AbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

    //loop through avtivable abilities in the Ability spec array
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {

        //check if AbilitySpec has exact match to InputTag
	    if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
	    {
            //bool switched when plyer input is pressed
            AbilitySpecInputPressed(AbilitySpec);

            //if ability is not already active
		    if (!AbilitySpec.IsActive())
		    {
                //try to use ability
                TryActivateAbility(AbilitySpec.Handle);
		    }
	    }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

    //loop through avtivable abilities in the Ability spec array
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        //check if AbilitySpec has exact match to InputTag
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            //bool switched when plyer input is pressed
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

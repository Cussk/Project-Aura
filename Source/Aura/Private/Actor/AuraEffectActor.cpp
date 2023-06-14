// Copyright Cuss Programming


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	//same as below, but doesn't account for if Interface is not implemented and an Actor with AbilitySystemComponent
	/*IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	if (ASCInterface) //check null
	{
		ASCInterface->GetAbilitySystemComponent();
	}*/

	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystemComponent == nullptr)
	{
		return;
	}


	check(GameplayEffectClass); //check null
	//context for effect
	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//Effect spec
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);

	//active effect to be applied
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get() /*Get data from Effect spec handle and dereference for FGameplayEffectSpec value*/);

	//get duration type of effect as bool
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//map of ActiveEffects and AbilitySystemComponents
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
	}

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	//apply instant effect on overlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	//apply duration effect on overlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	//apply infinite effect on overlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	//apply instant effect on end overlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	//apply duration effect on end overlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	//apply infinite effect on end overlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//get target actors ability system component
		UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAbilitySystemComponent)) return;


		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		//loop through map to see if matching pair of values
		for (auto HandlePair : ActiveEffectHandles)
		{
			//if matching Ability system component is in map
			if (TargetAbilitySystemComponent == HandlePair.Value)
			{
				//remove gameplay effect linked to ability system component
				TargetAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);

				//add ActiveGameplayEffect key to array of effects to be removed from map after looping is complete
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		//loop through array of map values stored
		for (auto& Handle : HandlesToRemove)
		{
			//remove ActiveEffectHandle from map
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



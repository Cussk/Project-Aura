// Copyright Cuss Programming


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include <AbilitySystemGlobals.h>

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine); //check null

	
	UAuraAssetManager* AssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	//return dereferenced AssetManager 
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//initialize gameplay tags that have been defined
	FAuraGameplayTags::InitializeNativeGameplayTags();

	//This is required to use Target Date, initializes Gameplay Ability system global data
	UAbilitySystemGlobals::Get().InitGlobalData();
}

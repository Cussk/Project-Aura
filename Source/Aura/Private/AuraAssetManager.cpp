// Copyright Cuss Programming


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

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
}

// Copyright Cuss Programming

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	//gets one and only instance of AssetManager
	static UAuraAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};

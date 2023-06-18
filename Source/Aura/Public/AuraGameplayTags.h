// Copyright Cuss Programming

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura Gameplay Tags
 *
 * Singleton containing Native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:

	//gets one and only instance of GameplayTags
	static const FAuraGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();
protected:

private:

	static FAuraGameplayTags GameplayTags;

};

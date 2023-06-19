// Copyright Cuss Programming


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	//loop through input actions in AbilityInputActions array
	for (const FAuraInputAction Action : AbilityInputActions)
	{
		//not null and tag matches
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return  Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

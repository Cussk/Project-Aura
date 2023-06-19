// Copyright Cuss Programming


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	//loop through attribute info array
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		//if Info has exact matching attribute tag return the struct info
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound == true)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	//if no matching tags found return empty array call
	return FAuraAttributeInfo();
}

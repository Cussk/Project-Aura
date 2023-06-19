// Copyright Cuss Programming


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//get attribute set
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo); // check null

	//loop through map to get all matching GameplayTag and Attribute delegate pairs
	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		//get the changed value for corresponding map pair and broadcast name and updated value
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//get attribute set
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo); // check null

	/*//get tag from FAuraGameplayTags Singleton
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);

	//Attribute value to value from AttributeSet
	Info.AttributeValue = AuraAttributeSet->GetStrength();

	//Broadcast contained Info to Listeners
	AttributeInfoDelegate.Broadcast(Info);*/

	//loop through map to get all matching GameplayTag and Attribute delegate pairs and broadcast name and value
	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& Attribute) const
{
	//get attribute tag from map
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	//get attributes numerical value from function pointer
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);

	//Broadcast contained Info to Listeners
	AttributeInfoDelegate.Broadcast(Info);
}

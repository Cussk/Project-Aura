// Copyright Cuss Programming


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//get vigor attribute
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//define attribute source from gameplay effect
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;

	//adds vigor to array of captured definitions
	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//data used for evaluating captured attribute
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.0f;

	//gets Vigor value at the time of capture
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParameters, Vigor);

	//clamps Vigor to never be negative
	Vigor = FMath::Max<float>(Vigor, 0.0f);

	//cast to interface of source of effect
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	float BaseValue = 80.0f;
	float AttributeMultiplier = 2.5f;
	float LevelMultiplier = 10.0f;

	return BaseValue + (AttributeMultiplier * Vigor) + (LevelMultiplier * PlayerLevel);
}

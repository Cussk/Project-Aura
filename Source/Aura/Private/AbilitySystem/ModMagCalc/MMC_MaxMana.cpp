// Copyright Cuss Programming


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	//get intelligence attribute
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	//define attribute source from gameplay effect
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	//adds intelligence to array of captured definitions
	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//data used for evaluating captured attribute
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.0f;

	//gets intelligence value at the time of capture
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluationParameters, Intelligence);

	//clamps Intelligence to never be negative
	Intelligence = FMath::Max(Intelligence, 0.0f);

	//cast to interface of source of effect
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	float BaseValue = 50.0f;
	float AttributeMultiplier = 2.5f;
	float LevelMultiplier = 15.0f;

	return BaseValue + (AttributeMultiplier * Intelligence) + (LevelMultiplier * PlayerLevel);
}

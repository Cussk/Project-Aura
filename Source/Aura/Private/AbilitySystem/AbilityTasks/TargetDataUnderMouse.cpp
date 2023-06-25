// Copyright Cuss Programming


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	//create new ability task
	UTargetDataUnderMouse* MyObject = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObject;
}

void UTargetDataUnderMouse::Activate()
{

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//On server, listen for target data

		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		//binds delegate to callback function
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//did the delegate get called already?
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		//if delegate was not called with client RPC
		if (!bCalledDelegate)
		{
			//wait for RPC to arrive then call delegate
			SetWaitingOnRemotePlayerData();
		}
	}

}

//locally controlled
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//everything within this function can be predicted, to synchronize client and server data
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//get player controller
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();

	//get hit result where cursor is in world
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;

	//create new data for hit result
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	//adds target data to the handle
	DataHandle.Add(Data);

	//sends the target data to the server
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	//decide if delegate should be broadcast
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//send delegate to listeners
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	/* MUST INITIALIZE UAbilitySystemGlobals::Get().InitGlobalData() IN ASSET MANAGER FOR TARGET DATA CACHE TO WORK */

	//clears replicated data cache when received by server
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	//decide if delegate should be broadcast
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//send delegate to listeners
		ValidData.Broadcast(DataHandle);
	}
}


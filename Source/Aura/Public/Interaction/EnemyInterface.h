// Copyright Cuss Programming

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()
		
public:

	//pure virtual function (Abstract), must be derived
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;
};

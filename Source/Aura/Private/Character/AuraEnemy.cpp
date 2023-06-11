// Copyright Cuss Programming


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	//Set mesh to block cursor visibility when hovering over
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//set value of stencil
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::HighlightActor()
{
	//activate custom depth for character outline
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnhighlightActor()
{
	//deactivate custom depth for character outline
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

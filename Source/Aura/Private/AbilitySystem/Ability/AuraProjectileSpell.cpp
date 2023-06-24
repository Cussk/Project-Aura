// Copyright Cuss Programming


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    const bool bIsServer = HasAuthority(&ActivationInfo);

    if (!bIsServer) return;

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
    if (CombatInterface) //check null
    {
        //get socket location on weapon
        const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

        //spawn trasform set a weapon socket loaction
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);

        //TODO: SEt projectile rotation

        //spawn projectile info
        AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
            ProjectileClass,
            SpawnTransform,
            GetOwningActorFromActorInfo(),
            Cast<APawn>(GetOwningActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        //TODO: Give projectile a gameplay effect spec to do damage

        //actually spawn projectile
        Projectile->FinishSpawning(SpawnTransform);
    }

}

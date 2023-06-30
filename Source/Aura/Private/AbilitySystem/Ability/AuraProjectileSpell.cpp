// Copyright Cuss Programming


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
    //check if calling from server
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();;

    if (!bIsServer) return;

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
    if (CombatInterface) //check null
    {
        //get socket location on weapon
        const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

        //get rotation between projectile spawn point and target location
        FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
        //keeps projectile parallel to ground
        Rotation.Pitch = 0.0f;

        //spawn trasform set a weapon socket loaction
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);

        //Set projectile rotation
        SpawnTransform.SetRotation(Rotation.Quaternion());

        //spawn projectile info
        AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
            ProjectileClass,
            SpawnTransform,
            GetOwningActorFromActorInfo(),
            Cast<APawn>(GetOwningActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        // gameplay effect spec to do damage
        //get ability system component
        const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

        //make gameplay effect spec handle for DamageEffectClass
        const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext());

        //get gameplay tags
        FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

        //get damage value from curve table based on ability level
        const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

        //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Firebolt Damage: %f"), ScaledDamage));

        //key value pair gameplay tag and magnitude for damage value
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

        //set DamageEffectSpecHandle
        Projectile->DamageEffectSpecHandle = SpecHandle;

        //actually spawn projectile
        Projectile->FinishSpawning(SpawnTransform);
    }
}

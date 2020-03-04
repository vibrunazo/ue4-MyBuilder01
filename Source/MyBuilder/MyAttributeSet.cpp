// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "IGetHit.h"

UMyAttributeSet::UMyAttributeSet()
    : Health(100.0f)
    , MaxHealth(100.0f)
{
}

bool UMyAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData & Data)
{
    if (GetHealth() <= 0)
    {
        return false;
    }
    return true;
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
    // Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
        // Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
        IIGetHit* HeWhoGetsHit = Cast<IIGetHit>(TargetActor);
        if (HeWhoGetsHit)
        {
            HeWhoGetsHit->Execute_OnDamaged (TargetActor);
            if (GetHealth() == 0)
            {
                HeWhoGetsHit->Execute_OnDie (TargetActor);
            }
        }
    }
}
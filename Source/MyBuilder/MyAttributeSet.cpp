// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "IGetHit.h"

UMyAttributeSet::UMyAttributeSet()
    : MaxHealth(100.0f)
    , Health(100.0f)
{
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
        if (GetHealth() == 0)
        {
            IIGetHit* TheInterface = Cast<IIGetHit>(TargetActor);
            if (TheInterface)
            {
                TheInterface->Execute_OnDie (TargetActor);
            }
        }
    }
}
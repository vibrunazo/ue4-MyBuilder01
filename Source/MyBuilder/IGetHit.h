// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayEffectTypes.h"
#include "IGetHit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGetHit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYBUILDER_API IIGetHit
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnGetHitByEffect(FGameplayEffectSpecHandle NewEffect);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnGetHitByEffects(const TArray<FGameplayEffectSpecHandle>& NewEffects);
	// void OnGetHitByEffect(TSubclassOf<class UGameplayEffect> NewEffect);
	// const TArray<FGameplayEffectSpecHandle>& NewEffect
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnDamaged(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnDie();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	bool IsAlive();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnHitPause(float Duration);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	int GetTeam();

};

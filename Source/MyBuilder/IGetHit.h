// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
	void OnGetHitByEffect(TSubclassOf<class UGameplayEffect> NewEffect);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GetHit")
	void OnDie();

};
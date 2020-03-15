// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYBUILDER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	void ResetValues();

	UPROPERTY()
	class UMyAttributeSet* AttributeSetBase;

	bool bInitialized = false;
	float PlayerHealth = 100.0f;
	
};

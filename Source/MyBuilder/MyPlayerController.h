// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYBUILDER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Player")
	void OnPlayerDead();
	void OnPlayerDead_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Player")
	void OnWin();
	void OnWin_Implementation();

private:
	void SpawnDefaultPawn();

	TSubclassOf<APawn> DefaultPawnClass;
	
};

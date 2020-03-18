// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomDataAsset.h"
#include "RoomSettings.generated.h"

UCLASS(Blueprintable)
class MYBUILDER_API ARoomSettings : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomSettings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	uint8 GetRoomDifficulty();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ERoomType RoomType = ERoomType::Empty;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	uint8 RoomDifficulty = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsWalled = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsDoored = false;

};

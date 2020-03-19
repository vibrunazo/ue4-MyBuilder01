// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelLoader.generated.h"

UCLASS()
class MYBUILDER_API ALevelLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoader();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Level Loader")
	ULevelStreaming* OnCreatedLevel(FName LevelName);

	TArray<FAssetData> AssetDataList;
	TArray<class URoomDataAsset*> RoomList;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GenerateLevels();
	void GenerateRoom();
	void SetDataListFromRegistry();


};

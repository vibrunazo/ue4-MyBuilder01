// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/LevelStreaming.h"
#include "AssetRegistryModule.h"
#include "RoomDataAsset.h"
#include "LevelLoader.h"

// Sets default values
ALevelLoader::ALevelLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelLoader::BeginPlay()
{
	Super::BeginPlay();

	GenerateLevels();
}

// Called every frame
void ALevelLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelLoader::GenerateLevels()
{
	SetDataListFromRegistry();

	for (uint8 i = 0; i < 4; i++)
	{
		GenerateRoom();
	}
}

void ALevelLoader::SetDataListFromRegistry()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> OutAssets;
	const UClass* Class = URoomDataAsset::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(Class->GetFName(), OutAssets);
	AssetDataList = OutAssets;
	for (auto &&AssetData : AssetDataList)
	{
		URoomDataAsset* NewRoom = Cast<URoomDataAsset>(AssetData.GetAsset());
		if (NewRoom)
		{
			RoomList.Add(NewRoom);
		}
	}
	
}

void ALevelLoader::GenerateRoom()
{
	URoomDataAsset* NewRoom = RoomList[FMath::RandRange(0, RoomList.Num() - 1)];

	OnCreatedLevel(NewRoom->LevelAddress);
}
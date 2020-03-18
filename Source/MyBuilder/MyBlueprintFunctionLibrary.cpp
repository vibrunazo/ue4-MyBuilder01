// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"

TArray<AActor*> UMyBlueprintFunctionLibrary::GetActorsFromLevel(ULevel* LoadedLevel)
{
    // FString LevelName = LoadedLevel->GetName();
    // if (!ensure(StreamedLevel != nullptr)) return {};
    // FString LevelName = StreamedLevel->GetName();
    // ULevel* LoadedLevel = StreamedLevel->GetLoadedLevel();
    if (!ensure(LoadedLevel != nullptr)) return {};
    FString LevelName = LoadedLevel->GetName();

    UE_LOG(LogTemp, Warning, TEXT("Loaded Level: %s"), *LevelName);
    TArray<AActor*> result = {};
    result = LoadedLevel->Actors;
    return result;
}
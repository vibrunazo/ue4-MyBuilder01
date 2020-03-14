// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMyPlayerController::AMyPlayerController()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MyDefaultPawn"));
	DefaultPawnClass = PlayerPawnBPClass.Class;
}

void AMyPlayerController::OnPlayerDead_Implementation()
{
    SpawnDefaultPawn();
}

void AMyPlayerController::OnWin_Implementation()
{
    SpawnDefaultPawn();
}

void AMyPlayerController::SpawnDefaultPawn()
{
    APawn* MyPawn = GetPawn();
    MyPawn->DetachFromControllerPendingDestroy();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, MyPawn->GetActorLocation(), MyPawn->GetActorRotation(), SpawnParams);
    Possess(NewPawn);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGameInstance.h"
#include "MyAttributeSet.h"
#include "MyBuilderCharacter.h"

AMyPlayerController::AMyPlayerController()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MyDefaultPawn"));
	DefaultPawnClass = PlayerPawnBPClass.Class;
}

void AMyPlayerController::OnPlayerDead_Implementation()
{
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (!ensure(GI != nullptr)) return;
    GI->ResetValues();
    SpawnDefaultPawn();
}

void AMyPlayerController::OnWin_Implementation(FName NextLevel)
{
    APawn* MyPawn = GetPawn();
    AMyBuilderCharacter* MyChar = Cast<AMyBuilderCharacter>(MyPawn);
    if (!ensure(MyChar != nullptr)) return;
    if (!ensure(MyChar->AttributeSetBase != nullptr)) return;
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (!ensure(GI != nullptr)) return;
    GI->PlayerHealth = MyChar->AttributeSetBase->GetHealth();
    SpawnDefaultPawn();
}

void AMyPlayerController::SpawnDefaultPawn()
{
    APawn* MyPawn = GetPawn();
    ACharacter* MyChar = Cast<ACharacter>(MyPawn);
    if (MyChar)
    {
        MyChar->StopAnimMontage();
    }
    MyPawn->DetachFromControllerPendingDestroy();
    MyPawn->DisableInput(nullptr);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, MyPawn->GetActorLocation(), MyPawn->GetActorRotation(), SpawnParams);
    Possess(NewPawn);
}
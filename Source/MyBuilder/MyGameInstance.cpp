// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "MyAttributeSet.h"


UMyGameInstance::UMyGameInstance()
{
    // AttributeSetBase = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSetBase"));
    // if (!ensure(AttributeSetBase != nullptr)) return;
    // AttributeSetBase->SetHealth(55.0f);
    // UE_LOG(LogTemp, Warning, TEXT("ini game instance, health is %f"), PlayerHealth);
    
}

void UMyGameInstance::ResetValues()
{
    bInitialized = false;
    PlayerHealth = 100.0f;
}
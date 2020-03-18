// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSettings.h"

// Sets default values
ARoomSettings::ARoomSettings()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomSettings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

uint8 ARoomSettings::GetRoomDifficulty()
{
	return RoomDifficulty;
}
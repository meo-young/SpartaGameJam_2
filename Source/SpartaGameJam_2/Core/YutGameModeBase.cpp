// Fill out your copyright notice in the Description page of Project Settings.


#include "YutGameModeBase.h"
#include "TileManager.h"

void AYutGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TileManager = NewObject<UTileManager>(this, TileManagerClass, FName("TileManager"), EObjectFlags::RF_Transient);

	check(TileManager);
}

void AYutGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (TileManager)
	{
		TileManager->CreateStage();
	}
}

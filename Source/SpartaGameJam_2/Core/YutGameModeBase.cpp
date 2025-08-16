// Fill out your copyright notice in the Description page of Project Settings.

#include "YutGameModeBase.h"
#include "StageSubsystem.h"
#include "TileManager.h"
#include "YutManager.h"

void AYutGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TileManager = NewObject<UTileManager>(this, TileManagerClass, FName("TileManager"), EObjectFlags::RF_Transient);
	YutManager = NewObject<UYutManager>(this, YutManagerClass, FName("YutManager"), EObjectFlags::RF_Transient);

	check(TileManager);
	check(YutManager);
	
	TurnCount = 0;
}

void AYutGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (!TileManager)
	{
		return;
	}

	TileManager->CreateStage();

	GetGameInstance()->GetSubsystem<UStageSubsystem>()->StartStage();
}
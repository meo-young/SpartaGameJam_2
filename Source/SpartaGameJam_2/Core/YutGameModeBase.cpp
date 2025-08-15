// Fill out your copyright notice in the Description page of Project Settings.


#include "YutGameModeBase.h"
#include "TileManager.h"

void AYutGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TileManager = NewObject<UTileManager>(this, TileManagerClass, FName("TileManager"), EObjectFlags::RF_Transient);

	check(TileManager);

	TrunCount = 0;
}

void AYutGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (!TileManager)
	{
		return;
	}

	TileManager->CreateStage();

	StartTurn();
}

void AYutGameModeBase::StartTurn()
{

	// 턴 종료
	++TrunCount;

	if (0 == TrunCount % 3)
	{
		TileManager->RotationStage();
	}
}

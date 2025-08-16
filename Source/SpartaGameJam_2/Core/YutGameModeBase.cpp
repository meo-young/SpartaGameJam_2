// Fill out your copyright notice in the Description page of Project Settings.


#include "YutGameModeBase.h"
#include "TileManager.h"

void AYutGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TileManager = NewObject<UTileManager>(this, TileManagerClass, FName("TileManager"), EObjectFlags::RF_Transient);

	check(TileManager);

	TrunCount = 0;

	// 매턴 먼저 시작 할 대상 선택
	// true인 경우 유저가 먼저 시작하도록 델리게이트에 먼저 바인딩한다.
	bool bIsPlayerFirst = FMath::RandBool();
	if (bIsPlayerFirst)
	{
		OnAction.AddDynamic(this, &AYutGameModeBase::PlayerTurn);
		OnAction.AddDynamic(this, &AYutGameModeBase::AITurn);
	}
	else
	{
		OnAction.AddDynamic(this, &AYutGameModeBase::AITurn);
		OnAction.AddDynamic(this, &AYutGameModeBase::PlayerTurn);
	}
}

void AYutGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (!TileManager)
	{
		return;
	}

	TileManager->CreateStage();

	HandleTurn();
}

void AYutGameModeBase::HandleTurn()
{
	// 바인딩 된 함수 호출하여 턴 진행
	OnAction.Broadcast();

	// 턴 종료
	++TrunCount;

	if (0 == TrunCount % 3)
	{
		TileManager->RotationStage();
	}
}

void AYutGameModeBase::PlayerTurn()
{
	// 플레이어 행동
}

void AYutGameModeBase::AITurn()
{
	// AI 행동
}

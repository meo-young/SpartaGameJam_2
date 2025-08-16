#include "StageSubsystem.h"

#include "DdakjiCharacter.h"
#include "EngineUtils.h"
#include "YutGameModeBase.h"
#include "YutManager.h"
#include "TileManager.h"

void UStageSubsystem::StartStage()
{
	// 1. 선 턴 소유자를 결정합니다.
	SetStartingTurn();

	bIsPlayerTurn = false;

	// 2. 스테이지 로직을 전개합니다.
	UpdateStage();
}

void UStageSubsystem::SetStartingTurn()
{
	bIsPlayerTurn = FMath::RandBool() ? 1 : 0;
}

void UStageSubsystem::UpdateStage()
{
	AYutGameModeBase* GameMode = Cast<AYutGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->YutManager->StartNewTurn();
	
	// 턴 소유주에 따른 행동 로직을 전개합니다.
	if (bIsPlayerTurn)
	{
		HandlePlayerTurn();
	}
	else
	{
		HandleAITurn();
	}
}

void UStageSubsystem::HandleAITurn()
{
	UE_LOG(LogTemp, Warning, TEXT("AI 턴입니다"));
	
	// 1. 윷을 던집니다.
	// @TODO : 윷 던지는 애니메이션, 윷 결과값에 따른 윷 모델링 조정 필요
	AYutGameModeBase* GameMode = Cast<AYutGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->YutManager->StartYutThrow(true);
	}
	

	// 6. 턴 종료 델리게이트를 호출합니다.
	if (OnTurnEndedDelegate.IsBound())
	{
		OnTurnEndedDelegate.Broadcast();
	}
}

void UStageSubsystem::HandlePlayerTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("플레이어 턴입니다"));

	// 1. 윷을 던지는 버튼이 활성화 됩니다.
	if (OnPlayerTurnDelegate.IsBound())
	{
		OnPlayerTurnDelegate.Broadcast();
	}


	// @TODO : 버튼을 누르면 해당 버튼을 비활성화 합니다.
	// @TODO : 결과 값에 따라 행동 로직을 다르게 진행 합니다.
}

void UStageSubsystem::UpdateEndTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("호출됨"));
	// 1. 턴 수를 증가합니다.
	++TurnCount;

	bIsPlayerTurn = !bIsPlayerTurn;

	UpdateStage();
}

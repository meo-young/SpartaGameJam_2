#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageSubsystem.generated.h"

class ADdakjiCharacter;

UCLASS()
class SPARTAGAMEJAM_2_API UStageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** 스테이지를 시작하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void StartStage();

	/** 선 턴 소유자를 결정하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void SetStartingTurn();

	/** 현재 턴 소유자에 따라 스테이지 로직을 업데이트 하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void UpdateStage();

	/** AI Turn일 때 로직을 전개하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void HandleAITurn();

	/** Player Turn일 때 로직을 전개하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void HandlePlayerTurn();

	/** 각 딱지 캐릭터의 위치 값을 저장하는 변수입니다. */
	TMap<ADdakjiCharacter, FVector> CharacterPos;
	
	/** 현재 플레이어 턴인지 상태를 나타내는 변수입니다. */
	uint8 bIsPlayerTurn : 1 = false;
	
};

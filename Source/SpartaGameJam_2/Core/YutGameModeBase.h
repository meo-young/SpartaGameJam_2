// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YutGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAction);

class UTileManager;

UCLASS()
class SPARTAGAMEJAM_2_API AYutGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void StartPlay();

	void HandleTurn();

	void PlayerTurn();

	void AITurn();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TSubclassOf<UTileManager> TileManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TObjectPtr<UTileManager> TileManager;

	// 턴 수
	int32 TrunCount;

public:
	// 턴 진행
	UPROPERTY(BlueprintAssignable)
	FOnAction OnAction;
};

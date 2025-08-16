// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YutGameModeBase.generated.h"

class USpawnManager;
class UStageSubsystem;
class UYutManager;
class UTileManager;

UCLASS()
class SPARTAGAMEJAM_2_API AYutGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void StartPlay();

	void StartTurn();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TSubclassOf<UTileManager> TileManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TObjectPtr<UTileManager> TileManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TSubclassOf<UYutManager> YutManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TObjectPtr<UYutManager> YutManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TSubclassOf<USpawnManager> SpawnManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpawnManager> SpawnManager;

	// 턴 수
	int32 TurnCount;
};

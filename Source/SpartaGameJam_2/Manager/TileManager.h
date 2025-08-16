// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileManager.generated.h"

class AYutTile;

UCLASS(BlueprintType, Blueprintable)
class SPARTAGAMEJAM_2_API UTileManager : public UObject
{
	GENERATED_BODY()

public:
	UTileManager();
	
	// 스테이지 생성 함수
	void CreateStage();

	// 스테이지 회전 함수
	void RotationStage();

	// 이동 가능 위치 반환


private:
	// 외곽 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Size", meta = (AllowPrivateAccess = true))
	int32 OutlineSize;

	// 내부 길이
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Size", meta = (AllowPrivateAccess = true))
	int32 InnerLength;

	// 중심으로부터의 외곽 발판의 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Size", meta = (AllowPrivateAccess = true))
	float OutlineRadius;

	// 기본 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> NormalTileClass;

	// 교차점 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> CrossTileClass;

	// 시작 및 도착 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> StartTileClass;

	// 중앙 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> CenterTileClass;

	// 트랩 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> TrapTileClass;

	// 행운 발판 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<AYutTile> LuckyTileClass;
	
	// 트랩 발판 수
	int32 TrapTileCount;

	// 행운 발판 수
	int32 LuckyTileCount;

	// 외곽 액터 캐싱 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AYutTile>> OutlineTileList;

	// 내부 액터 캐싱 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AYutTile>> InnerTileList;

	// 교차로 위치 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = true))
	TMap<TObjectPtr<AYutTile>, int32> CrossTileList;
};

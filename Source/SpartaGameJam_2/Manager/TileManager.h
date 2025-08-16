// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileManager.generated.h"

class AYutTile;
class ADdakjiCharacter;

UENUM()
enum class ETeamType : uint8
{
	NONE,
	Team1,
	Team2,
};

UENUM()
enum class EMoveDirection : uint8
{
	Outer,
	OutertoCenter,
	CentertoOuter,
};

struct FPawnData
{
public:
	ETeamType OwnerTeam;

	int32 LocationIndex;

	// 목표 노드의 방향
	EMoveDirection Direction;

	// 목표 노드 인덱스
	int32 TargetIndex;

	// 이동 예정인 경로
	TArray<int32> MovePath;

	// 처음 출발지에서 시작할 때 예외 처리를 위한 변수
	bool bIsSpawn = true;
};

UCLASS(BlueprintType, Blueprintable)
class SPARTAGAMEJAM_2_API UTileManager : public UObject
{
	GENERATED_BODY()

public:
	UTileManager();

public:
	// 스테이지 생성 함수
	void CreateStage();

	// 스테이지 회전 함수
	void RotationStage();

	// 이동 가능 위치 반환
	UFUNCTION(BlueprintCallable)
	int32 GetMovableTileIndex(ADdakjiCharacter* TargetPawn, int32 MoveRange);

	UFUNCTION(BlueprintCallable)
	void MoveTile(ADdakjiCharacter* TargetPawn, int32 MoveRange);

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

#pragma region TileClass
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
	
#pragma endregion

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

#pragma region YutPawn
public:
	// 말 스폰 및 캐싱
	void InitYutPawn();

private:
	// 플레이어 및 AI의 말 캐싱
	TMap<TObjectPtr<ADdakjiCharacter>, FPawnData> YutPawnArr;

	// 캐릭터 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YutPawn|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<ADdakjiCharacter> YutPawnClass;
#pragma endregion
};

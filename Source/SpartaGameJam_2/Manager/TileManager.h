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

	// 처음 출발지에서 시작할 때 도착 관련 예외 처리를 위한 변수
	bool bIsStart = false;
};

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
	UFUNCTION(BlueprintCallable)
	int32 GetMovableTileIndex(ADdakjiCharacter* TargetPawn, int32 MoveRange);

	UFUNCTION(BlueprintCallable)
	void MoveTile(ADdakjiCharacter* TargetPawn, int32 MoveRange);

	UFUNCTION(BlueprintCallable)
	void MoveTile_Index(int32 TargetCharacterIndex, int32 MoveRange);

	// 도착할 수 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable)
	bool GoalCheck(ADdakjiCharacter* TargetPawn, TArray<int32> MoveArray);

	// 다른 말을 잡을 수 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable)
	bool WillGrappleCheck(ADdakjiCharacter* TargetPawn, TArray<int32> MoveArray);

	// 말을 시작 지점으로 날리는 함수
	UFUNCTION(BlueprintCallable)
	void GrappleTarget(ADdakjiCharacter* TargetPawn);

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

#pragma region TileList
	// 외곽 액터 캐싱 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|List", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AYutTile>> OutlineTileList;

	// 내부 액터 캐싱 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|List", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AYutTile>> InnerTileList;

	// 교차로 위치 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|List", meta = (AllowPrivateAccess = true))
	TMap<TObjectPtr<AYutTile>, int32> CrossTileList;

	// 트랩 위치 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|List", meta = (AllowPrivateAccess = true))
	TMap<int32, TObjectPtr<AYutTile>> TrapTileList;

	// 행운 위치 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|List", meta = (AllowPrivateAccess = true))
	TMap<int32, TObjectPtr<AYutTile>> LuckyTileList;

#pragma endregion

#pragma region YutPawn
public:
	// 말 스폰 및 캐싱
	void InitYutPawn();

	FVector GetPading();

public:
	// 플레이어 및 AI의 말 캐싱
	TMap<TObjectPtr<ADdakjiCharacter>, FPawnData> YutPawnArr;

	// 캐릭터 클래스 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YutPawn|Class", meta = (AllowPrivateAccess = true))
	TSubclassOf<ADdakjiCharacter> YutPawnClass;

	// 폰 위치에 대한 패딩
	FVector Pading = { 0.f, 0.f, 16.f };
#pragma endregion


};

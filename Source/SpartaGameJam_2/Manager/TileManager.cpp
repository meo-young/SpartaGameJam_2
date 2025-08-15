// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "YutTile.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

UTileManager::UTileManager()
{
	// 변수 초기화
	OutlineSize = 20;
	InnerLength = 2;

	OutlineRadius = 200.f;

	TrapTileCount = 3;
	LuckyTileCount = 3;
}

void UTileManager::CreateStage()
{
	TArray<AActor*> SpawnCenterArr;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnCenterArr);

	// 스폰을 위한 타겟 포인터가 없다면 실행 취소
	if (!SpawnCenterArr.IsValidIndex(0))
	{
		return;
	}

	FVector CenterLocation = SpawnCenterArr[0]->GetTargetLocation();

	// 발판 노드 동적 생성 로직
	TArray<FVector> SpawnLocationArr;

	// 외곽 노드 위치 설정
	const float AngleStep = 360.f / OutlineSize;

	for (int32 i = 0; i < OutlineSize; ++i)
	{
		float SpawnAngleDegrees = AngleStep * i;
		float SpawnRadians = FMath::DegreesToRadians(SpawnAngleDegrees);

		float X = CenterLocation.X + OutlineRadius * FMath::Cos(SpawnRadians);
		float Y = CenterLocation.Y + OutlineRadius * FMath::Sin(SpawnRadians);
		float Z = CenterLocation.Z;

		FVector NewSpawnLocation(X, Y, Z);
		
		SpawnLocationArr.Add(NewSpawnLocation);
	}
	
	// 내부 노드 위치 설정

	// 아래, 오른쪽, 위, 왼쪽
	FVector Directions[4] = {
		FVector(1, 0, 0),
		FVector(0, 1, 0),
		FVector(-1, 0, 0),
		FVector(0, -1, 0),
	};

	float StepDis = OutlineRadius / (InnerLength + 1);

	for (auto Dir : Directions)
	{
		for (int32 i = 1; i <= InnerLength; ++i)
		{
			FVector NewSpawnLocation = CenterLocation + (Dir * StepDis * i);

			SpawnLocationArr.Add(NewSpawnLocation);
		}
	}

	// 중앙 노드 위치 설정
	SpawnLocationArr.Add(CenterLocation);

	// 행운, 트랩 발판 스폰
	// 시작, 중앙 위치는 일반 발판
	// 각 교차점도 일반 발판으로 설정하기
	//TODO: 꼭짓점은 색이 다른 일반 발판으로 설정하기
	
	// 행운 및 트랩 발판을 설정할 인덱스
	TArray<int32> SpecialTileIndexArr;

	for (int32 i = 1; i < SpawnLocationArr.Num() - 1; ++i)
	{
		if (i % 5 != 0)
		{
			SpecialTileIndexArr.Add(i);
		}
	}

	for (int32 i = 0; i < SpecialTileIndexArr.Num(); ++i)
	{
		int32 SwapIndex = FMath::RandRange(i, SpecialTileIndexArr.Num() - 1);
		SpecialTileIndexArr.Swap(i, SwapIndex);
	}

	SpecialTileIndexArr.SetNum(TrapTileCount + LuckyTileCount);

	int32 RemainTrapTileCount = TrapTileCount;
	int32 RemainLuckyTileCount = LuckyTileCount;


	for (int32 i = 0; i < SpawnLocationArr.Num(); ++i)
	{
		// 내부 중심 노드 생성
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AYutTile* Spawned = nullptr;

		// 시작점 발판 스폰
		if (i == 0 && StartTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(StartTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);
		}
		// 중앙 발판 스폰
		else if (i == SpawnLocationArr.Num() - 1 && CenterTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(CenterTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);
		}
		// 교차점 발판 스폰
		else if (i < OutlineSize && i % (OutlineSize / 4) == 0 && CrossTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(CrossTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);
		}
		// 트랩 발판 스폰
		else if (SpecialTileIndexArr.Contains(i) && RemainTrapTileCount && TrapTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(TrapTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);

			--RemainTrapTileCount;
		}
		// 행운 발판 스폰
		else if (SpecialTileIndexArr.Contains(i) && RemainLuckyTileCount && LuckyTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(LuckyTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);

			--RemainLuckyTileCount;
		}
		// 일반 발판 스폰
		else if (NormalTileClass)
		{
			Spawned = GetWorld()->SpawnActor<AYutTile>(NormalTileClass, SpawnLocationArr[i], FRotator::ZeroRotator, Params);
		}

		// 액터를 스폰하지 못한 경우
		if (Spawned == nullptr)
		{
			continue;
		}

		if (i < OutlineSize)
		{
			OutlineTileList.Add(Spawned);
		}
		else
		{
			InnerTileList.Add(Spawned);
		}
	}

	// 교차로가 될 노드를 캐싱
	// 각 노드의 연결 관계 저장
	if (InnerTileList.Num() == (InnerLength * 4 + 1) &&
		InnerTileList.IsValidIndex(1) &&
		InnerTileList.IsValidIndex(3) &&
		InnerTileList.IsValidIndex(5) &&
		InnerTileList.IsValidIndex(7))
	{
		CrossTileList = {
		{ InnerTileList[1], 0 },
		{ InnerTileList[3], 5 },
		{ InnerTileList[5], 10 },
		{ InnerTileList[7], 15 },
		};
	}
}

void UTileManager::RotationStage()
{
	for (auto& CrossTile : CrossTileList)
	{
		CrossTile.Value += 1;

		CrossTile.Value %= OutlineSize;
	}

	//TODO: 실제로 위치가 갱신되도록 구현해야한다.
}

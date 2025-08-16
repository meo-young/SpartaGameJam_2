// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "YutTile.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "DdakjiCharacter.h"
#include "YutCenterTile.h"

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

	FVector Directions[4] = {
		FVector(1, 0, 0),
		FVector(0, 1, 0),
		FVector(-1, 0, 0),
		FVector(0, -1, 0),
	};

	float StepDis = OutlineRadius / (InnerLength + 1);

	for (auto Dir : Directions)
	{
		for (int32 i = InnerLength; i > 0; --i)
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
	if (InnerTileList.Num() == (InnerLength * 4 + 1))
	{
		CrossTileList = {
		{ OutlineTileList[0], 0 },
		{ OutlineTileList[5], 1 * InnerLength },
		{ OutlineTileList[10], 2 * InnerLength },
		{ OutlineTileList[15], 3 * InnerLength },
		};
	}
}

void UTileManager::RotationStage()
{
	// 값의 연결을 말 진행과 반대 방향으로 회전시킨다.
	TArray<int32> TargetIndexArr;
	for (auto& CrossTile : CrossTileList)
	{
		int32 TargetIndex = OutlineTileList.FindLast(CrossTile.Key) - 1;
		if (TargetIndex < 0)
		{
			TargetIndex = OutlineSize - 1;
		}
	}

	CrossTileList.Empty(4);
	for (int i = 0; i < 4; ++i)
	{
		CrossTileList.Add(OutlineTileList[TargetIndexArr[i]], i * InnerLength);
	}


	//TODO: 실제로 위치가 갱신되도록 구현해야한다.
}

int32 UTileManager::GetMovableTileIndex(ADdakjiCharacter* TargetPawn, int32 MoveRange)
{
	int32 Result = 0;

	ADdakjiCharacter* CurDdakjiCharacter = nullptr;

	FPawnData* TargetPawnData = YutPawnArr.Find(TargetPawn);
	if (!TargetPawnData)
	{
		return 0;
	}
	// 타겟 말의 위치를 찾는다.
	int32 PawnLocationIndex = TargetPawnData->LocationIndex;

	// 타겟 말의 위치에 해당하는 타일을 찾는다.
	AYutTile* LocationTile = OutlineTileList[PawnLocationIndex];

	// 내부로 이동할 수 있는 경우의 인덱스
	int* InlineTileIndexPtr = CrossTileList.Find(LocationTile);

	int32 RemainMoveRange = MoveRange;
	while (RemainMoveRange)
	{
		// 내부로 이동할 수 없고, 외부에서 이동해야할 경우
		if (TargetPawnData->Direction == EMoveDirection::Outer)
		{
			PawnLocationIndex += RemainMoveRange;
			// 앞으로 간 경우
			PawnLocationIndex = (PawnLocationIndex >= 20) ? 0 : PawnLocationIndex;
			// 뒤로 간 경우
			PawnLocationIndex = (PawnLocationIndex < 0) ? 20 - PawnLocationIndex : PawnLocationIndex;

			Result = PawnLocationIndex;
			RemainMoveRange = 0;

			// 도착 위치에서 내부로 들어갈 수 있는지 확인 후 목표 방향 및 위치 변경
			if (InlineTileIndexPtr)
			{
				TargetPawnData->Direction = EMoveDirection::OutertoCenter;
				TargetPawnData->TargetIndex = InnerLength * 4;
			}
			else
			{
				TargetPawnData->Direction = EMoveDirection::Outer;
				TargetPawnData->TargetIndex = OutlineSize - 1;
			}
		}
		// 외부에서 내부로 이동할 수 있는 경우
		else if (TargetPawnData->Direction == EMoveDirection::OutertoCenter)
		{
			// 외부에서 내부로 이동하려는 경우 내부로 한칸 이동
			if (InlineTileIndexPtr)
			{
				PawnLocationIndex = *InlineTileIndexPtr;

				RemainMoveRange -= 1;
			}

			// 가운데 위치까지와의 거리
			int32 CurLocation = PawnLocationIndex % ((PawnLocationIndex != 0) ? PawnLocationIndex / InnerLength : InnerLength);
			int32 CenterDistance = InnerLength - CurLocation;

			// 중앙까지 이동할 수 있는 횟수를 구한다.
			int32 CurMoveRange = RemainMoveRange - CenterDistance;
			// 중앙까지 이동할 수 있는 경우
			if (CurMoveRange >= 0)
			{
				RemainMoveRange = CurMoveRange;

				PawnLocationIndex = InnerLength * 4;

				// 목표 방향 변경
				TargetPawnData->Direction = EMoveDirection::OutertoCenter;

				// 목표 위치 변경
				// 가운데가 바라보고 있는 방향
				AYutCenterTile* YutCenterTile = Cast<AYutCenterTile>(InnerTileList[InnerLength * 4]);
				if (!YutCenterTile)
				{
					return 0;
				}
				int32 IndexDirection = YutCenterTile->GetIndexDirection();
				TargetPawnData->TargetIndex = IndexDirection;

				if (CurMoveRange == 0)
				{
					Result = PawnLocationIndex;
				}
			}
			// 중앙까지 이동할 수 없는 경우
			else if (CurMoveRange < 0)
			{
				// 이동할 수 있는 만큼 이동
				Result = CenterDistance - FMath::Abs(CurMoveRange);

				RemainMoveRange = 0;

				// 목표 방향 변경
				TargetPawnData->Direction = EMoveDirection::OutertoCenter;

				// 이번 이동에서 더이상 내부로 이동하지 않도록 변경
				InlineTileIndexPtr = nullptr;
			}
		}
		// 내부에서 외부로 이동하는 경우
		if (TargetPawnData->Direction == EMoveDirection::CentertoOuter)
		{
			// 중앙 발판을 밟고있는 경우 
			if (PawnLocationIndex == InnerTileList[InnerLength * 4])
			{
				// 목표 인덱스 변경
				// 가운데가 바라보고 있는 방향
				AYutCenterTile* YutCenterTile = Cast<AYutCenterTile>(InnerTileList[InnerLength * 4]);
				if (!YutCenterTile)
				{
					return 0;
				}
				int32 IndexDirection = YutCenterTile->GetIndexDirection();
				TargetPawnData->TargetIndex = IndexDirection;

				// 중앙 발판을 밟고있는 경우 중앙에서 나온다.
				PawnLocationIndex = IndexDirection + InnerLength - 1;

				RemainMoveRange -= 1;
			}

			// 외부 위치까지와의 거리
			int32 CenterDistance = (PawnLocationIndex != 0) ? PawnLocationIndex % InnerLength + 1 : 1;

			// 외부까지 이동할 수 있는 횟수를 구한다.
			int32 CurMoveRange = RemainMoveRange - CenterDistance;
			// 외부까지 이동할 수 있는 경우
			if (CurMoveRange >= 0)
			{
				RemainMoveRange = CurMoveRange;

				PawnLocationIndex = TargetPawnData->TargetIndex;

				// 목표 방향 변경
				TargetPawnData->Direction = EMoveDirection::OutertoCenter;

				if (CurMoveRange == 0)
				{
					Result = PawnLocationIndex;
				}
			}
			// 외부까지 이동할 수 없는 경우
			else if (CurMoveRange < 0)
			{
				// 최대한 이동
				Result = TargetPawnData->TargetIndex + CurMoveRange;

				RemainMoveRange = 0;
			}
		}
	}

	return Result;
}

void UTileManager::MoveTile(ADdakjiCharacter* TargetPawn, int32 MoveRange)
{
	int32 TargetIndex = GetMovableTileIndex(TargetPawn, MoveRange);

	for (int i = 0; i < MoveRange; ++i)
	{
		// 이동하는 함수 호출
	}
}

void UTileManager::InitYutPawn()
{
	if (!OutlineTileList.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("Invelid Index Access at %s (Line: %d) in %s"), TEXT(__FUNCTION__), __LINE__, TEXT(__FILE__));
		return;
	}

	FVector YutSpawnLocation = OutlineTileList[0]->GetActorLocation();

	// 플레이어 수 2, 2개의 말을 사용하기 때문에 4개의 말 생성 및 캐싱
	for (int i = 0; i < 4; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ADdakjiCharacter* NewPawn = GetWorld()->SpawnActor<ADdakjiCharacter>(ADdakjiCharacter::StaticClass(), YutSpawnLocation, FRotator::ZeroRotator, Params);

		FPawnData NewPawnData;

		if (i < 2)
		{
			NewPawnData = { ETeamType::Team1, 0, EMoveDirection::OutertoCenter };
		}
		else
		{
			NewPawnData = { ETeamType::Team2, 0, EMoveDirection::OutertoCenter };
		}

		YutPawnArr.Add({ NewPawn, NewPawnData });
	}
}

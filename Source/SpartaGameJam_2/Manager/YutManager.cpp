#include "YutManager.h"

#include "DdakjiCharacter.h"
#include "EngineUtils.h"
#include "StageSubsystem.h"
#include "TileManager.h"
#include "YutGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

void UYutManager::Initialize()
{
	LoadYutData();

	UE_LOG(LogTemp, Error, TEXT("UYutManager::InitializeShop"));
}

void UYutManager::LoadYutData()
{
	if (!YutDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("YutDataTable not set in YutManager"));
		return;
	}
	
	CachedYutData.Empty();
	TArray<FName> RowNames = YutDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		if (FYutResultData* YutData = YutDataTable->FindRow<FYutResultData>(RowName, TEXT("")))
		{
			CachedYutData.Add(*YutData);
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Loaded %d YutData"), CachedYutData.Num());
}

void UYutManager::StartNewTurn()
{
	AvailableYuts.Empty();
	bCanThrow = true;

	UE_LOG(LogTemp, Error, TEXT("UYutManager::StartNewTurn"));
}

void UYutManager::StartYutThrow(bool bIsAI)
{
	bIsAITurn = bIsAI;
	
	if (SpawnedYutActor)
	{
		YutMeshes.Empty();
		SpawnedYutActor->Destroy();
		SpawnedYutActor = nullptr;
	}

	SpawnYutActor();
	ApplyPhysicsImpulse();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UYutManager::ShowYutResult, 2.5f);
}


void UYutManager::ShowYutResult()
{
	int32 Result = CalculateYutResult();
	FYutResultData YutResult = GetYutData(Result);
	UE_LOG(LogTemp, Error, TEXT("Yut Result: %s (%d)"), *YutResult.ResultName, YutResult.YutResult);
	
	bCanThrow = YutResult.bCanThrowAgain;
	AvailableYuts.Add(YutResult);
	OnThrowFinished.Broadcast(YutResult, AvailableYuts);

	if (bIsAITurn) // AI 턴일 경우
	{
		if (bCanThrow)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UYutManager::ProcessNextThrow, 3.0f); 
		}
		else
		{
			TArray<FYutResultData> YutResultDatas = EndTurn();
			AYutGameModeBase* GameMode = Cast<AYutGameModeBase>(GetWorld()->GetAuthGameMode());

			if (GameMode)
			{
				float AccumulatedDelay = 0.f; // 지금까지 대기한 시간 누적

				for (const FYutResultData& YutResultData : YutResultDatas)
				{
					uint8 YutResultValue = YutResultData.YutResult;

					// 이동에 필요한 시간 계산 (한 칸당 0.5초라면)
					float MoveDuration = YutResultValue * 0.5f;

					if (AccumulatedDelay <= 0.0f)
					{
						GameMode->TileManager->MoveTile_Index(0, YutResultValue);
						UE_LOG(LogTemp, Warning, TEXT("YUT RESULT: %u"), YutResultValue);
						break;
					}
					else
					{
						// AccumulatedDelay 후에 실행되도록 예약
						FTimerHandle MoveTimerHandle;
						GetWorld()->GetTimerManager().SetTimer(
							MoveTimerHandle,
							FTimerDelegate::CreateLambda([GameMode, YutResultValue]()
							{
								GameMode->TileManager->MoveTile_Index(0, YutResultValue);
								UE_LOG(LogTemp, Warning, TEXT("YUT RESULT: %u"), YutResultValue);
							}),
							AccumulatedDelay,
							false
						);	
					}

					OnYutUsed.Broadcast(YutResultData);

					// 다음 이동을 위해 대기시간 누적
					AccumulatedDelay += MoveDuration;
				}
			}

			UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
			StageSubsystem->UpdateEndTurn();
		}
	}
	else // 플레이어 턴일 경우
	{
		//OnThrowFinished.Broadcast(YutResult, AvailableYuts);
		if (!bCanThrow)
		{
			EndTurn();
		}
	}
}

void UYutManager::ProcessNextThrow()
{
	StartYutThrow(true);
}

TArray<FYutResultData> UYutManager::EndTurn()
{
	return AvailableYuts;
}

FYutResultData UYutManager::GetYutData(int32 YutResult)
{
	for (const FYutResultData& YutData : CachedYutData)
	{
		if (YutData.YutResult == YutResult)
		{
			FYutResultData Result = YutData;
			Result.bCanThrowAgain = (YutResult == 4 || YutResult == 5);
			return Result;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Could not find YutData for result: %d"), YutResult);
	FYutResultData DefaultResult;
	DefaultResult.YutResult = 1;
	DefaultResult.ResultName = TEXT("도");
	DefaultResult.bCanThrowAgain = false;
	return DefaultResult;
}

void UYutManager::SpawnYutActor()
{
	FActorSpawnParameters SpawnParams;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnedYutActor = GetWorld()->SpawnActor<AActor>(YutActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedYutActor)
	{
		YutMeshes.Empty(); 
        
		TArray<UStaticMeshComponent*> FoundMeshes;
		SpawnedYutActor->GetComponents<UStaticMeshComponent>(FoundMeshes);
		for (UStaticMeshComponent* Mesh : FoundMeshes)
		{
			YutMeshes.Add(Mesh);
		}
	}
}

void UYutManager::ApplyPhysicsImpulse()
{
	for (UStaticMeshComponent* YutMesh : YutMeshes)
	{
		YutMesh->SetSimulatePhysics(true);
		
		FVector ImpulseDirection = FVector::UpVector;
		float ImpulseStrength = FMath::RandRange(500.0f, 800.0f);
		YutMesh->AddImpulse(ImpulseDirection * ImpulseStrength, NAME_None, true);

		// 랜덤 회전
		FVector AngularImpulse = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(200.0f, 400.0f);
		YutMesh->AddAngularImpulseInDegrees(AngularImpulse, NAME_None, true);
	}
}

int32 UYutManager::CalculateYutResult()
{
	int32 UpCnt = 0;
	UStaticMeshComponent* UpwardYut = nullptr;
	
	for (UStaticMeshComponent* YutMesh : YutMeshes)
	{
		YutMesh->SetSimulatePhysics(false);
		
		FVector MeshLocation = YutMesh->GetComponentLocation();
		FVector TraceStart = MeshLocation;
		FVector TraceEnd = MeshLocation - YutMesh->GetForwardVector() * 200.0f;
		
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			ECC_GameTraceChannel1
		);

		if (bHit)
		{
			UpCnt++;
			if (UpCnt == 1)
			{
				UpwardYut = YutMesh;
			}
			else 
			{
				UpwardYut = nullptr; 
			}
		}
	}
	
	if (UpCnt == 0) return 5; // 모
	if (UpCnt == 2) return 2; // 개
	if (UpCnt == 3) return 3; // 걸
	if (UpCnt == 4) return 4; // 윷
	
	if (UpCnt == 1)
	{
		if (UpwardYut && UpwardYut->ComponentHasTag(TEXT("BackDo")))
		{
			return -1; // 빽도
		}
		else
		{
			return 1; // 도
		}
	}

	return 1;
}

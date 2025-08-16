#include "YutManager.h"

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

void UYutManager::StartYutThrow()
{
	int32 Result = CalculateRandomYut();
	FYutResultData YutResult = GetYutData(Result);
	bCanThrow = YutResult.bCanThrowAgain;
	
	AvailableYuts.Add(YutResult);
	OnThrowFinished.Broadcast(YutResult, AvailableYuts);

	if (!bCanThrow)
	{
		EndTurn();
	}
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
	DefaultResult.Probability = 20.0f;
	DefaultResult.bCanThrowAgain = false;
	return DefaultResult;
}

int32 UYutManager::CalculateRandomYut()
{
	float TotalWeight = 0.0f;
	for (const FYutResultData& YutData : CachedYutData)
	{
		TotalWeight += YutData.Probability;
	}
	
	float RandomWeight = FMath::FRand() * TotalWeight;
	
	float CurrentProbability = 0.0f;
	for (const FYutResultData& YutData : CachedYutData)
	{
		CurrentProbability += YutData.Probability;
		if (RandomWeight <= CurrentProbability)
		{
			UE_LOG(LogTemp, Error, TEXT("Yut Result: %s (%d)"), *YutData.ResultName, YutData.YutResult);
			return YutData.YutResult;
		}
	}
    
	return 1;
}
#include "Yut/YutManager.h"

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
		if (FYutResultData* ShopItem = YutDataTable->FindRow<FYutResultData>(RowName, TEXT("")))
		{
			CachedYutData.Add(*ShopItem);
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Loaded %d YutData"), CachedYutData.Num());
}

void UYutManager::StartYutThrow()
{
	int32 Result = CalculateRandomYut();
	bool bCanThrowAgain = CanThrowAgain(Result);

	UE_LOG(LogTemp, Log, TEXT("Yut Result: %d. Can Throw Again: %s"), Result, bCanThrowAgain ? TEXT("true") : TEXT("false"));

	// TODO : 델리게이트로 UI 활성화 
	OnThrowFinished.Broadcast(Result, bCanThrowAgain);
}

bool UYutManager::CanThrowAgain(int32 YutResult)
{
	return (YutResult == 4 || YutResult == 5);
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
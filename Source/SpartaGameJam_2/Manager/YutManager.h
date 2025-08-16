#pragma once

#include "CoreMinimal.h"
#include "YutManager.generated.h"

USTRUCT(BlueprintType)
struct FYutResultData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 YutResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResultName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanThrowAgain;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnThrowFinished, FYutResultData, YutResult, const TArray<FYutResultData>&, AvailableYuts);
UCLASS(Blueprintable)
class SPARTAGAMEJAM_2_API UYutManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	void LoadYutData();

	UFUNCTION(BlueprintCallable)
	void StartYutThrow();

	bool CanThrowAgain(int32 YutResult);
	FYutResultData GetYutData(int32 YutResult);
	int32 CalculateRandomYut();
	
	TArray<FYutResultData> CachedYutData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* YutDataTable;

	bool bIsInProgress;

	UPROPERTY(BlueprintAssignable)
	FOnThrowFinished OnThrowFinished;

	TArray<FYutResultData> AvailableYuts;
};

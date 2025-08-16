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

	UFUNCTION(BlueprintCallable)
	void StartNewTurn();
	
	void LoadYutData();
	TArray<FYutResultData> EndTurn();

	UFUNCTION(BlueprintCallable)
	void StartYutThrow();
	
	FYutResultData GetYutData(int32 YutResult);
	int32 CalculateRandomYut();
	void SpawnYutActor();
	void ApplyPhysicsImpulse();
	
	UFUNCTION(BlueprintCallable)
	bool CanThrowYut() const { return bCanThrow; }
	
	TArray<FYutResultData> CachedYutData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* YutDataTable;
	
	bool bCanThrow;

	UPROPERTY(BlueprintAssignable)
	FOnThrowFinished OnThrowFinished;

	TArray<FYutResultData> AvailableYuts;
	
	UPROPERTY()
	TArray<UStaticMeshComponent*> YutMeshes;
	
	UPROPERTY()
	AActor* SpawnedYutActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> YutActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;
};

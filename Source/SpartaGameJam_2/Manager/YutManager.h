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
	UTexture2D* ResultTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanThrowAgain;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYutUsed, const FYutResultData&, UsedYutResult);
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
	void StartYutThrow(bool bIsAI);

	void ShowYutResult();
	void ProcessNextThrow();
	
	FYutResultData GetYutData(int32 YutResult);
	void SpawnYutActor();
	void ApplyPhysicsImpulse();
	int32 CalculateYutResult();
	
	UFUNCTION(BlueprintCallable)
	bool CanThrowYut() const { return bCanThrow; }
	
	TArray<FYutResultData> CachedYutData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* YutDataTable;
	
	bool bCanThrow;
	bool bIsAITurn;

	UPROPERTY(BlueprintAssignable)
	FOnYutUsed OnYutUsed;

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

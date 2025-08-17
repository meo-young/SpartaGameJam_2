#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VFXSubsystem.generated.h"

enum class EVFX : uint8;
struct FVFXTableRowBase;

UCLASS()
class SPARTAGAMEJAM_2_API UVFXSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SpawnVFX(EVFX VFX, const FVector SpawnLocation);

	/** VFX 데이터 테이블에 대한 참조 변수입니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> VFXDataTable;

	/** VFX 데이터 테이블에서 추출한 모든 행에 대한 참조 변수입니다. */
	TArray<FVFXTableRowBase*> VFXDataTableRows;
	
	
};

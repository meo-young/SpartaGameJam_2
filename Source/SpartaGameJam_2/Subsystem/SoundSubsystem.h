#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundSubsystem.generated.h"

enum class ESFX : uint8;
enum class EBGM : uint8;
struct FSFXTableRowBase;
struct FBGMTableRowBase;

UCLASS()
class SPARTAGAMEJAM_2_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** SFX를 재생하는 함수입니다. */
	void PlaySFX(ESFX SFX);

	/** BGM을 재생하는 함수입니다. */
	void PlayBGM(EBGM BGM);

	/** BGM을 중단하는 함수입니다. */
	void StopBGM();

	/** SFX 데이터 테이블에 대한 참조 변수입니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> SFXDataTable;

	/** SFX 데이터 테이블에서 추출한 모든 행 데이터 변수입니다. */
	TArray<FSFXTableRowBase*> SFXDataRows;

	/** BGM 데이터 테이블에 대한 참조 변수입니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> BGMDataTable;

	/** BGM 데이터 테이블에서 추출한 모든 행 데이터 변수입니다. */
	TArray<FBGMTableRowBase*> BGMDataRows;

	/** BGM 재생을 위한 AudioComponent 입니다. */
	UPROPERTY()
	UAudioComponent* AudioComponent;
	
};

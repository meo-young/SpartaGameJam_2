#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "DdakjiCharacter.generated.h"

UCLASS()
class SPARTAGAMEJAM_2_API ADdakjiCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	/** 목표 위치로 캐릭터를 점프시키는 함수입니다. */ 
	UFUNCTION(BlueprintCallable)
	void JumpToLocation(FVector Location);

	/** 점프 곡선 커브 변수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="변수")
	UCurveFloat* JumpSquareCurve;

	/** 점프하는 시간을 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="변수")
	float JumpDuration;

	/** 최대 점프 높이를 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="변수")
	float MaxHeight = 150.0f;
	
	/** 점프 중인지 상태를 나타내는 변수입니다. */
	uint8 bIsJumping : 1 = false;

	/** 점프를 시작하는 위치에 대한 변수입니다. */
	FVector StartPos;

	/** 점프 목적지 위치에 대한 변수입니다. */
	FVector TargetPos;

	/** 점프 중인 시간을 측정하는 변수입니다. */
	float ElapsedTime = 0.f;
	
};

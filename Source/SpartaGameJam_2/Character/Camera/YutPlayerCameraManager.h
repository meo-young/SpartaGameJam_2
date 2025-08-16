#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "YutPlayerCameraManager.generated.h"

UCLASS()
class SPARTAGAMEJAM_2_API AYutPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	/** 카메라 쉐이크 기본 클래스 입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	TSubclassOf<UCameraShakeBase> CameraShakeBaseClass;
	
};

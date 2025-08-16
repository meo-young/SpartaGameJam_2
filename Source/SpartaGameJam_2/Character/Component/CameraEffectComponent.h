#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAGAMEJAM_2_API UCameraEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraEffectComponent();
	
	/** 플레이어의 카메라를 Duration 동안 흔드는 함수입니다. */	
	void ShakeCamera(float Duration);

	/** 카메라 쉐이크 기본 클래스 입니다. */
	UPROPERTY()
	TSubclassOf<UCameraShakeBase> CatchCameraShakeBaseClass;
		
};

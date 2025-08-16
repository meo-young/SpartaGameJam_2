#include "Character/Component/CameraEffectComponent.h"

UCameraEffectComponent::UCameraEffectComponent()
{
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CSB_Catch(TEXT("/Game/_Sparta/Blueprints/CameraShakeBase/CSB_Catch"));
	if (CSB_Catch.Succeeded())
	{
		CatchCameraShakeBaseClass = CSB_Catch.Class;
	}
}

void UCameraEffectComponent::ShakeCamera(float Duration)
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CatchCameraShakeBaseClass);
}

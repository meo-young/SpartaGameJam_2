#include "Character/Camera/YutPlayerCameraManager.h"

void AYutPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	StartCameraShake(CameraShakeBaseClass);
}

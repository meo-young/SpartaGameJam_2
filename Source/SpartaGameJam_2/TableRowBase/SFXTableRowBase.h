#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "SFXTableRowBase.generated.h"

enum class ESFX : uint8;

USTRUCT(BlueprintType)
struct FSFXTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ESFX SFX;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> Cue;
	
};

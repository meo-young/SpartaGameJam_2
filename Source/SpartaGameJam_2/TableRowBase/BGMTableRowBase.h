#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "BGMTableRowBase.generated.h"

enum class EBGM : uint8;

USTRUCT(BlueprintType)
struct FBGMTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EBGM BGM;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> Cue;
	
};

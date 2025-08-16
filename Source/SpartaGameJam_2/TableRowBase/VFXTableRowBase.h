#pragma once

#include "CoreMinimal.h"
#include "VFXTableRowBase.generated.h"

enum class EVFX : uint8;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FVFXTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EVFX VFX;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;
	
};
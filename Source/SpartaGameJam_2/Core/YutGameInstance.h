#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "YutGameInstance.generated.h"

UCLASS()
class SPARTAGAMEJAM_2_API UYutGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "변수")
	uint8 bCanTitleActive : 1 = true;
	
};

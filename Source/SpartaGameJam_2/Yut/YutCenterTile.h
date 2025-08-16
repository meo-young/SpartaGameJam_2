// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Yut/YutTile.h"
#include "YutCenterTile.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAGAMEJAM_2_API AYutCenterTile : public AYutTile
{
	GENERATED_BODY()
	
public:
	AYutCenterTile();

	int32 GetIndexDirection() const;

private:
	int32 DirectionIndex;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "YutCenterTile.h"

AYutCenterTile::AYutCenterTile()
{
	DirectionIndex = 0;
}

int32 AYutCenterTile::GetIndexDirection() const
{
	return DirectionIndex;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "YutTile.h"

AYutTile::AYutTile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(SceneComp);
}

void AYutTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AYutTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


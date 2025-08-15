// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YutTile.generated.h"

UCLASS()
class SPARTAGAMEJAM_2_API AYutTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYutTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;
};

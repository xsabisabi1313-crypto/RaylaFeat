// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawn.generated.h"

UCLASS()
class RAYLA_FEAT_API AUnitSpawn : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AUnitSpawn();

	// Blueprintから呼び出せるスポーン関数
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnMyUnit(TSubclassOf<AActor> UnitClassToSpawn, FVector SpawnLocation);
};
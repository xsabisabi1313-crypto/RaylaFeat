// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.h"

#include "UnitSpawn.generated.h"

UCLASS()
class RAYLA_FEAT_API AUnitSpawn : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AUnitSpawn();

	// 味方をスポーンする関数
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnMyUnit(FVector SpawnLocation, FIntPoint SpawnGridPos);

	//現在選択されているカードの実態
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	ACard* CurrentSelectedCard = nullptr;


};
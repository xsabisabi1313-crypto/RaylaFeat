// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.h" // EPlayerSide を使うためにインクルード
class AGameManager;

#include "BaseBuilding.generated.h"

UCLASS()
class RAYLA_FEAT_API ABaseBuilding : public AActor
{
	GENERATED_BODY()

public:
	ABaseBuilding();

	// HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStatus")
	int32 HP = 20;


	// どちらの陣営か（Player / Enemy）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStatus")
	EPlayerSide PlayerSide;

	
	// グリッド上の位置(常にYが-1か7のため、ここでは定義しない。

	// ダメージを受ける関数
	UFUNCTION(BlueprintCallable, Category = "BaseStatus")
	void TakeBuildingDamage(int32 DamageAmount);


};
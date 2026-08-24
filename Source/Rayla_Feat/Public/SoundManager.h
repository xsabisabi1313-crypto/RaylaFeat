// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

UCLASS()
class RAYLA_FEAT_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	

	// --- ① 音の変数（エディタでアサイン用） ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_CardClick = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_UnitSpawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_ClickPhaseEndButton = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_ClickUnit = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_ClickMoveReservePos = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_UnitMove = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_BattleStart = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_AttackBaseBuilding = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_AttackUnit = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_Win = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|SE")
	USoundBase* SE_Lose = nullptr;


	// --- SE再生する関数 ---

	UFUNCTION(BlueprintCallable, Category = "Sound|Func")
	void PlaySE(USoundBase* Sound);

	//

};

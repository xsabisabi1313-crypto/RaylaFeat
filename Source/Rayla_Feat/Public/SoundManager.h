// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class RAYLA_FEAT_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// サウンド一覧のデータテーブル（Blueprintから参照・設定可能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UDataTable* SoundDataTable;

	// 名前（行名）を指定してSEを再生する関数
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySoundByName(FName SoundRowName);
};


USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 音のファイル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundAsset = nullptr;

	// 音量（デフォルトは1.0）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float VolumeMultiplier = 1.0f;


};
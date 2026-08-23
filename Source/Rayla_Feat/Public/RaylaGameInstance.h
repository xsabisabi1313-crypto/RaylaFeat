// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RaylaGameInstance.generated.h"


UENUM(BlueprintType)
enum class ETeam : uint8
{
	Akazukin    UMETA(DisplayName = "Akazukin"),
	Momotaro  UMETA(DisplayName = "Momotaro"),
};


UCLASS()
class RAYLA_FEAT_API URaylaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 例：プレイヤーが選んだチームのデータ（文字列や、チームのID、構造体など）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	ETeam SelectedTeam = ETeam::Momotaro;
};

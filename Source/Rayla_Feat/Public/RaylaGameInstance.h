// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RaylaGameInstance.generated.h"


UENUM(BlueprintType)
enum class PlayerSelectedUnitTeam : uint8
{
	Akazukin       UMETA(DisplayName = "Akazukin"),
	Momotaro     UMETA(DisplayName = "Momotaro"),
};

UCLASS()
class RAYLA_FEAT_API URaylaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 例：プレイヤーが選んだチームのデータ（文字列や、チームのID、構造体など）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	PlayerSelectedUnitTeam SelectedTeam = PlayerSelectedUnitTeam::Akazukin;

	// enumの変数を保持するプロパティ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStatus")
	PlayerSelectedUnitTeam CurrentGameState;

	// 初期化時などに値を設定できるようにする関数
	UFUNCTION(BlueprintCallable, Category = "GameStatus")
	void SetGameState(PlayerSelectedUnitTeam NewState);

	UFUNCTION(BlueprintCallable, Category = "GameStatus")
	PlayerSelectedUnitTeam GetGameState() const;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetSelectedTeam(PlayerSelectedUnitTeam NewTeam);
};

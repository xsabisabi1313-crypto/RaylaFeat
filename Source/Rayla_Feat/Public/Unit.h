// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"
//攻撃のパターン
UENUM(BlueprintType)
enum class EAtackPatterns : uint8
{
	Cross    UMETA(DisplayName = "Cross"),
	Forward  UMETA(DisplayName = "Forward"),
	All      UMETA(DisplayName = "All"),
	Diagonal UMETA(DisplayName = "Diagonal")
};


//移動のパターン
UENUM(BlueprintType)
enum class EMovePatterns : uint8
{
	Cross    UMETA(DisplayName = "Cross"),
};

//属性
UENUM(BlueprintType)
enum class EElementtype : uint8
{
	None   UMETA(DisplayName = "None"),
	Fire    UMETA(DisplayName = "Fire"),
	Water   UMETA(DisplayName = "Water"),
	Grass    UMETA(DisplayName = "Grass")

};

//敵か味方か
UENUM(BlueprintType)
enum class EPlayerSide : uint8
{
	Player    UMETA(DisplayName = "(Player)"),
	Enemy     UMETA(DisplayName = "(Enemy)"),
};

UCLASS()
class RAYLA_FEAT_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnit();

	//アクターがクリックされたとき呼ぶ関数
	UFUNCTION(BlueprintCallable, Category = "Unit")
	void OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
		

	// 1. 名前
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FText UnitName;

	// 2. コスト（召喚や出撃に必要なポイント）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Cost = 10;

	//現在のいる位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntPoint GridPos;

	//現在のいる位置を更新する関数
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void MoveToGrid(FIntPoint NewGridPos);

	// 生成したい移動範囲オブジェクトの「設計図（ブループリント）」を指定する変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<AActor> MovePatternObjClass;



	// 移動範囲オブジェクトを生成する関数
	UFUNCTION(BlueprintCallable, Category = "MovePatternObj")
	void SpawnMovePatternObject();


	//パワー
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Power = 0;

	//属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EElementtype Element = EElementtype::None;

	//移動パターン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EMovePatterns MovePattern = EMovePatterns::Cross;

	//攻撃パターン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EAtackPatterns AttackPattern = EAtackPatterns::Forward;

	//敵か味方か
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EPlayerSide PlayerSide = EPlayerSide::Player;


	// 現在の位置から攻撃できるマスを、攻撃パターンごとにすべて返す配列
	UFUNCTION(BlueprintCallable, Category = "Debug")
	TArray<FIntPoint> GetAvailableAttackPoss();

	// 現在の位置から移動予約が可能なマスを,パターンごとにすべて返す配列
	UFUNCTION(BlueprintCallable, Category = "MovePosFunc")
	TArray<FIntPoint> GetAvailableMovePoss();


	// マウスが乗ったとき・離れたときに呼ばれる関数
	UFUNCTION()
	void OnCursorBeginOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnCursorEndOver(UPrimitiveComponent* TouchedComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};

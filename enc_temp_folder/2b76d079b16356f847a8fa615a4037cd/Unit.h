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

//属性
UENUM(BlueprintType)
enum class EElementtype : uint8
{
	None   UMETA(DisplayName = "無属性"),
	Fire    UMETA(DisplayName = "炎"),
	Water   UMETA(DisplayName = "水"),
	Grass    UMETA(DisplayName = "草")

};
UCLASS()
class RAYLA_FEAT_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnit();

	UFUNCTION()
	void OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
		

	// 1. 名前
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FText UnitName;

	// 2. コスト（召喚や出撃に必要なポイント）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Cost = 10;

	//3.攻撃できる全てのマス
	UFUNCTION(BlueprintCallable, Category = "Battle")
	TArray<FIntPoint> CalculateAttackRange(
		int32 InTargetGridX,
		int32 InTargetGridY,
		int32 InPlayerSide,
		EAtackPatterns InAtackPattern,
		int32 InAtackRange
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EElementtype Element = EElementtype::None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};

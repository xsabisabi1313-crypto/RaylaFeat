// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UENUM(BlueprintType)
enum class CurrentPhase : uint8
{
	EGS_Spawn   UMETA(DisplayName = "Spawn"),
	EGS_MoveReserve    UMETA(DisplayName = "MoveReserve"),
	EGS_Move    UMETA(DisplayName = "Move"),
	EGS_Battle    UMETA(DisplayName = "Battle"),
	EGS_GameOver  UMETA(DisplayName = "Game Over")
};

UCLASS()
class RAYLA_FEAT_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 2. 定義した列挙型をメンバー変数として使う例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	CurrentPhase currentPhase;
};

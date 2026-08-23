// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.h"
#include "EnemyAI.generated.h"

UCLASS()
class RAYLA_FEAT_API AEnemyAI : public AActor
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AGameManager* GameManagerRef;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void ExecuteAITurn();

	void ProcessAISpawn();
	void ProcessAIMoveReserve();

};

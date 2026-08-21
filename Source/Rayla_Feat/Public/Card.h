// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.generated.h"

UCLASS()
class RAYLA_FEAT_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();
	UFUNCTION(BlueprintCallable, Category = "Card")
	void OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);


protected:


public:	

	// 召喚する味方ユニット
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TSubclassOf<AActor> UnitToSpawn = nullptr;


	//カードを沈ませたり浮かばせる関数
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardSelected(bool IsSelected);

};

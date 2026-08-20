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
	UFUNCTION()
	void OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// è¢ä´Ç∑ÇÈñ°ï˚ÉÜÉjÉbÉg
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TSubclassOf<AActor> UnitToSpawn = nullptr;

};

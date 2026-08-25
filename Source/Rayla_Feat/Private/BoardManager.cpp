// Fill out your copyright notice in the Description page of Project Settings.
#include "BoardManager.h"

// Called when the game starts or when spawned
void ABoardManager::BeginPlay()
{
	Super::BeginPlay();
	InitBoard();
}

// Called every frame
void ABoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardManager::InitBoard() {
	for (int y = 0; y < 7; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			ATile* SpawnedTile = GetWorld()->SpawnActor<ATile>(Tile, FVector(x * 100, y * 100, 0.0f), FRotator::ZeroRotator);
			SpawnedTile->GridXY = FIntPoint(x, y);
		}
	}

}


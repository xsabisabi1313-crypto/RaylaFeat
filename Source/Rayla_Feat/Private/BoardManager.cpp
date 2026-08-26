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

            AllTiles.Add(SpawnedTile);
		}
	}

}

// 渡されたGridPosを全て光らせる関数
void ABoardManager::ShowMovableRange(TArray<FIntPoint> GridPoss)
{
    // ループ変数をクラス名とかぶらない「EachTile」にする
    for (ATile* EachTile : AllTiles)
    {
        if (!EachTile) continue;

        FIntPoint TilePos = EachTile->GridXY;

        UE_LOG(LogTemp, Display, TEXT("今から光らせる！light!!!!!!!now!!!!!!!"));

        // 渡された座標リストの中に含まれていれば光らせ、違えば戻す
        if (GridPoss.Contains(TilePos))
        {
            UE_LOG(LogTemp, Display, TEXT("Onnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn"));
            EachTile->SetHighlight(true);
        }
        else
        {
            EachTile->SetHighlight(false);
        }
    }
}
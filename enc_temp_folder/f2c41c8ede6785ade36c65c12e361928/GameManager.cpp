// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}




//現在の位置から移動できるマスを、移動のパターンごとに変えて全て返す配列
void AGameManager::GetAvailableMovePositions(AUnit* TargetUnit)
{
	// 1. まずリストを綺麗にリセットする（前回の残りを消す）
	AvailableMovePositions.Empty();


	// 1. ユニットの現在のグリッド座標を取得
	FIntPoint CurrentPos = TargetUnit->GridPos;

	//ユニットの移動パターンを取得
	EMovePatterns MovePattern = TargetUnit->MovePattern;

	switch (MovePattern)
	{
	case EMovePatterns::Cross:
		AvailableMovePositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));//右
		AvailableMovePositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));//左
		AvailableMovePositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + 1)); // 上
		AvailableMovePositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - 1)); // 下

	default:
		break;
	}

}

bool AGameManager::IsValidMoveDestination(FIntPoint TargetGridPos)
{
	// 2. そのリストの中に、プレイヤーがクリックした座標（TargetGridPos）が含まれているかチェック！
	return AvailableMovePositions.Contains(TargetGridPos);
}
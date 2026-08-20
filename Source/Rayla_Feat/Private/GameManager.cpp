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
		break;

	default:
		break;
	}

}

bool AGameManager::IsValidMoveDestination(FIntPoint TargetGridPos)
{
	// 2. そのリストの中に、プレイヤーがクリックした座標（TargetGridPos）が含まれているかチェック！
	return AvailableMovePositions.Contains(TargetGridPos);
}

//実際にユニットを移動させる関数
void AGameManager::ExecuteMove()
{
	// 1. 動かすユニットが選択されているか、さらにポインタが有効（nullptrではない）か厳重にチェック！
	if (!SelectedUnit || !IsValid(SelectedUnit))
	{
		UE_LOG(LogTemp, Warning, TEXT("エラー：移動するユニットが選択されていないか、すでに無効です！"));
		return;
	}

	// ユニット側の位置を更新する（前作った MoveToGrid 関数などを活用）
	SelectedUnit->MoveToGrid(ReserveGridPos);

	//UE_LOG(LogTemp, Warning, TEXT("ユニットを移動させました！"));

	// 3. 移動が終わったあとの後片付け（必要に応じて）
	// 例：選択を解除したり、フェーズを次のターンに進めたりする
	SelectedUnit = nullptr;
}


void AGameManager::ExecuteBattle() {
	//まず、味方側

	// 1. まず味方側：操作（選択）しているユニットがちゃんといるかチェック
	if (!SelectedUnit || !IsValid(SelectedUnit))
	{
		UE_LOG(LogTemp, Warning, TEXT("戦闘：選択されている味方ユニットがいません。"));
		return;
	}

	// 2. 敵側を探す：先ほどのように、味方の攻撃範囲（AttackRangePriority）を使って敵を見つける
	AUnit* TargetEnemy = nullptr;

	// フィールドにいる全アクターから探す（または事前に用意したリストから）
	
	for (AActor* Actor : AllUnitsList)
	{
		AUnit* OtherUnit = Cast<AUnit>(Actor);
		if (!OtherUnit || OtherUnit == SelectedUnit) continue;

	}


	// 3. 敵が見つかった場合の処理
	if (TargetEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("戦闘発生！ 味方 が 敵を発見しました！"));

		// TODO: ここで属性の相性計算や、お互いのHPを減らす処理を呼び出す
		// 例: ResolveBattle(SelectedUnit, TargetEnemy);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("攻撃範囲内に敵はいませんでした。"));
	}
	//selectedunitを取得


	//敵側
}
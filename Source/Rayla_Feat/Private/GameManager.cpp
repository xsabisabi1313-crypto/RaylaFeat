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

void AGameManager::GetAvailableAttackPositions(AUnit* TargetUnit) {

	if (!TargetUnit) {
		return;
	}

	// 1. まずリストを綺麗にリセットする（前回の残りを消す）
	AvailableAttackPositions.Empty();

	// 1. ユニットの現在のグリッド座標を取得
	FIntPoint CurrentPos = TargetUnit->GridPos;

	//ユニットの攻撃パターンを取得
	EAtackPatterns AttackPattern = TargetUnit->AttackPattern;

	//敵か味方かによって、前後が変わる。味方ならYが-１、敵ならYが+1
	int32 ForwardDir = 1;
	if (TargetUnit->PlayerSide == EPlayerSide::Player)
	{
		ForwardDir = -1; // 味方はマイナス方向が「前」
	}
	else if (TargetUnit->PlayerSide == EPlayerSide::Enemy)
	{
		ForwardDir = 1;  // 敵はプラス方向が「前」
	}


	switch (AttackPattern)
	{
	case EAtackPatterns::Cross:
		// 十字方向（前後左右1マスずつなど）
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir)); // 前
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - ForwardDir)); // 後ろ
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));        // 左
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));        // 右
		break;

	case EAtackPatterns::Forward:
		// 「前」方向だけ攻撃できる
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir));
		break;

	case EAtackPatterns::All:
		// 周囲8マス全部など
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir)); // 前
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - ForwardDir)); // 後
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));            // 左
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));            // 右
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y + ForwardDir)); // 前左
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y + ForwardDir)); // 前右
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y - ForwardDir)); // 後左
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y - ForwardDir)); // 後右
		break;

	case EAtackPatterns::Diagonal:
		// 斜め4方向
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y + ForwardDir)); // 前左斜め
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y + ForwardDir)); // 前右斜め
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y - ForwardDir)); // 後ろ左斜め
		AvailableAttackPositions.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y - ForwardDir)); // 後ろ右斜め
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
	//SelectedUnit = nullptr;
}


void AGameManager::ExecuteBattle() {
	//まず、味方側
	AUnit* Attacker = SelectedUnit;
	GetAvailableAttackPositions(Attacker);
	// 【ループ①】攻撃できるマスの数だけ回す
	for (const FIntPoint& AttackPos : AvailableAttackPositions)
	{
		// 【ループ②】フィールドにいるすべてのキャラクターの数だけ回す
		for (AActor* Actor : AllUnitsList)
		{
			AUnit* OtherUnit = Cast<AUnit>(Actor);
			if (!OtherUnit || OtherUnit == Attacker) continue;

			// 自分と同じ陣営（味方同士）なら攻撃対象外
			if (OtherUnit->PlayerSide == Attacker->PlayerSide) continue;

			// 【判定】「攻撃できるマス」と「キャラの現在地」が一致するか！？
			if (OtherUnit->GridPos.X == AttackPos.X && OtherUnit->GridPos.Y == AttackPos.Y)
			{
				// 敵を発見！この瞬間に「こいつを殴る」と確定できる
				//return OtherUnit;
				GEngine->AddOnScreenDebugMessage(
					-1,                          // キー (-1なら古いメッセージを上書きせず毎回新しい行で表示)
					3.0f,                        // 表示する時間（秒）
					FColor::Yellow,              // 文字の色
					TEXT("敵を発見しししし")          // 表示したい文字列
				);
			}
		}
	}
}
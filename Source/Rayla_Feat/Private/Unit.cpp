// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- ▼ 重要！「クリックを検知する」設定を有効にする ▼ ---
	bEnableAutoLODGeneration = true;
}


void AUnit::BeginPlay()
{
	Super::BeginPlay();

	// 自分のルートコンポーネント（またはメッシュ・コリジョン）を取得してバインドする
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(RootComponent))
	{
		// マウスが乗ったとき
		PrimComp->OnBeginCursorOver.AddDynamic(this, &AUnit::OnCursorBeginOver);

		// マウスが離れたとき
		PrimComp->OnEndCursorOver.AddDynamic(this, &AUnit::OnCursorEndOver);
	}
}

// ユニットがクリックされたときの処理
void AUnit::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

	//味方じゃなかったら反応させない
	if (PlayerSide != EPlayerSide::Player) // 敵の場合
	{
		
		return;
	}


	// ワールド上のGameManagerを探して自分をセットする
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);
	if (!MyGameManager)return;
	MyGameManager->SelectedUnit = this;


	//MyGameManager->SelectedUnit = this;
	UE_LOG(LogTemp, Warning, TEXT("ユニットを選択しました: %s"), *GetName());
	if (MyGameManager->currentPhase == CurrentPhase::EGS_MoveReserve) {
		SpawnMovePatternObject();
		this->GetAvailableMovePoss();
	}
	
}


void AUnit::OnCursorBeginOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("マウスが乗った！: %s"), *GetName());
}

void AUnit::OnCursorEndOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("マウスが離れた！: %s"), *GetName());
}



//現在の位置を更新する
void AUnit::MoveToGrid(FIntPoint NewGridPos)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveNow！"));
	if (!IsValid(this)) return;

	GridPos = NewGridPos;

	FVector NewWorldLocation = FVector(GridPos.X * 100.0f, GridPos.Y * 100.0f, GetActorLocation().Z);

	// 3. アクターの実際の3D位置を更新する
	SetActorLocation(NewWorldLocation);
}

//移動できる範囲を生成する
void AUnit::SpawnMovePatternObject()
{

	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);


	if (MyGameManager->CurrentMovePatternObj) {
		MyGameManager->DeleteMoveRangeObj();
	}
	MyGameManager->CurrentMovePatternObj = GetWorld()->SpawnActor<AActor>(MovePatternObjClass, FVector(GridPos.X * 100, GridPos.Y * 100, 46), FRotator::ZeroRotator);
}




//攻撃可能なマスを計算して、AvailableAttackPositionsに格納する
TArray<FIntPoint> AUnit::GetAvailableAttackPoss() {

	if (!this) {
		return TArray<FIntPoint>();
	}

	TArray<FIntPoint> AvailableAttackPoss;

	// 1. ユニットの現在のグリッド座標を取得
	FIntPoint CurrentPos = this->GridPos;


	//敵か味方かによって、前後が変わる。味方ならYが-１、敵ならYが+1
	int32 ForwardDir = 1;
	if (this->PlayerSide == EPlayerSide::Player)
	{
		ForwardDir = -1; // 味方はマイナス方向が「前」
	}
	else if (this->PlayerSide == EPlayerSide::Enemy)
	{
		ForwardDir = 1;  // 敵はプラス方向が「前」
	}


	switch (this->AttackPattern)
	{
	case EAtackPatterns::Cross:
		// 十字方向（前後左右1マスずつなど）
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y));//自分の位置
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir)); // 前
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - ForwardDir)); // 後ろ
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));        // 左
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));        // 右
		break;

	case EAtackPatterns::Forward:
		// 「前」方向だけ攻撃できる
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y));
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir));
		break;

	case EAtackPatterns::All:
		// 周囲8マス全部など
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y));//自分の位置
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + ForwardDir)); // 前
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - ForwardDir)); // 後
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));            // 左
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));            // 右
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y + ForwardDir)); // 前左
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y + ForwardDir)); // 前右
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y - ForwardDir)); // 後左
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y - ForwardDir)); // 後右
		break;

	case EAtackPatterns::Diagonal:
		// 斜め4方向
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y));//自分の位置
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y + ForwardDir)); // 前左斜め
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y + ForwardDir)); // 前右斜め
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y - ForwardDir)); // 後ろ左斜め
		AvailableAttackPoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y - ForwardDir)); // 後ろ右斜め
		break;

	default:
		break;
	}

	return AvailableAttackPoss;
}


//現在の位置から移動できるマスを、移動のパターンごとに変えて全て返す配列
TArray<FIntPoint> AUnit::GetAvailableMovePoss()
{

	if (!this)return TArray<FIntPoint>();
	TArray<FIntPoint> AvailableMovePoss;

	// 1. ユニットの現在のグリッド座標を取得
	FIntPoint CurrentPos = this->GridPos;

	switch (this->MovePattern)
	{
	case EMovePatterns::Cross:
		AvailableMovePoss.Add(FIntPoint(CurrentPos.X + 1, CurrentPos.Y));//右
		AvailableMovePoss.Add(FIntPoint(CurrentPos.X - 1, CurrentPos.Y));//左
		AvailableMovePoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y + 1)); // 上
		AvailableMovePoss.Add(FIntPoint(CurrentPos.X, CurrentPos.Y - 1)); // 下
		break;

	default:
		break;
	}

	return AvailableMovePoss;

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "UnitSpawn.h"
#include "Card.h" // カードのヘッダーを読み込む

#include "Blueprint/UserWidget.h"       // CreateWidget や UUserWidget を使うため
#include "Kismet/GameplayStatics.h"     // UGameplayStatics::SetGamePaused を使うため
#include "GameFramework/PlayerController.h" // APlayerController を使うため（もしエラーが出る場合）

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



//プレイヤーがクリックした座標（TargetGridPos）が含まれているかチェック！
bool AGameManager::IsValidMoveDestination(FIntPoint TargetGridPos)
{
	return SelectedUnit->GetAvailableMovePoss().Contains(TargetGridPos);
}

//実際にユニットを移動させる
void AGameManager::ExecuteMove()
{
	if (SelectedEnemyUnit) {
		SelectedEnemyUnit->MoveToGrid(ReserveEnemyGridPos);
	}
	if (SelectedUnit) {
		SelectedUnit->MoveToGrid(ReserveGridPos);
	}

	SelectedUnit = nullptr;
	SelectedEnemyUnit = nullptr;
}

//バトル処理
//バトル処理
void AGameManager::ExecuteBattle(EPlayerSide AttackerSide)
{
	AUnit* Attacker = nullptr;
	if (AttackerSide == EPlayerSide::Enemy && IsValid(SelectedEnemyUnit)) {
		Attacker = SelectedEnemyUnit;
	}
	else if (AttackerSide == EPlayerSide::Player && IsValid(SelectedUnit)) {
		Attacker = SelectedUnit;
	}

	if (!IsValid(Attacker)) return;

	// 実際に破壊するアクターをためておくリスト
	TArray<AUnit*> UnitsToDestroy;
	//もう既にこのフェーズで戦ったかどうか
	bool isFought = false;

	// 【ループ①】攻撃できるマスの数だけ回す
	for (const FIntPoint& AttackPos : Attacker->GetAvailableAttackPoss())
	{
		// 【ループ②】フィールドにいるすべてのキャラクターの数だけ回す
		for (AActor* Actor : AllUnitsList)
		{
			if (!IsValid(Actor)) continue;

			AUnit* OtherUnit = Cast<AUnit>(Actor);
			if (!OtherUnit || OtherUnit == Attacker) continue;

			// 自分と同じ陣営（味方同士）なら攻撃対象外
			if (OtherUnit->PlayerSide == Attacker->PlayerSide) continue;

			// 【判定】「攻撃できるマス」と「キャラの現在地」が一致するか！？
			if (OtherUnit->GridPos.X == AttackPos.X && OtherUnit->GridPos.Y == AttackPos.Y)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					3.0f,
					FColor::Orange,
					TEXT("Battle！")
				);

				// ★修正：元の Power を直接書き換えないよう、一時的な戦闘力変数を作る！
				int32 AttackerPower = Attacker->Power;
				int32 OtherPower = OtherUnit->Power;

				// 属性相性による一時的なパワー補正
				if (Attacker->Element == EElementtype::Fire && OtherUnit->Element == EElementtype::Water) {
					OtherPower *= 2;
				}
				else if (Attacker->Element == EElementtype::Fire && OtherUnit->Element == EElementtype::Grass) {
					AttackerPower *= 2;
				}
				else if (Attacker->Element == EElementtype::Grass && OtherUnit->Element == EElementtype::Water) {
					AttackerPower *= 2;
				}
				// 必要に応じて他の相性（水→火、草→水など）もここに追加できます

				// 勝敗の判定（一時的なパワーで比較する）
				if (AttackerPower == OtherPower) {
					UnitsToDestroy.Add(Attacker);
					UnitsToDestroy.Add(OtherUnit);
				}
				else if (AttackerPower > OtherPower) {
					UnitsToDestroy.Add(OtherUnit);
				}
				else {
					UnitsToDestroy.Add(Attacker);
				}

				isFought = true;
				// 該当する相手を見つけたらループを抜ける
				break;
			}
		}
		if (UnitsToDestroy.Num() > 0) break;
	}

	// ループが終わった安全な場所で、まとめてリストから外し、破壊する
	for (AUnit* UnitToDestroy : UnitsToDestroy)
	{
		if (IsValid(UnitToDestroy))
		{
			AllUnitsList.RemoveSingle(UnitToDestroy);
			UnitToDestroy->Destroy();
		}
	}

	//もう戦ったなら、終了
	if (isFought)return;

	//まだ戦っていないなら、拠点が攻撃可能かどうか判定する。
// 1. AI側の拠点（(0,-1), (1,-1), (2,-1)）のどれかに入っているかチェック
	bool bHitEnemyBase = SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(0, -1)) ||
		SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(1, -1)) ||
		SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(2, -1));

	// 2. プレイヤーの拠点（(0,7), (1,7), (2,7)）のどれかに入っているかチェック
	bool bHitPlayerBase = SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(0, 7)) ||
		SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(1, 7)) ||
		SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(2, 7));

	// --- 判定とダメージ処理 ---

	// もし「攻撃側がプレイヤー」かつ「敵の拠点（Y=7側）のマスを攻撃範囲に捉えた」場合
	if (Attacker->PlayerSide == EPlayerSide::Player && bHitEnemyBase)
	{
		if (EnemyBase)
		{
			// 拠点にダメージを与える（ReceiveDamage関数を呼ぶ）
			EnemyBase->TakeBuildingDamage(4);
			UE_LOG(LogTemp, Warning, TEXT("プレイヤーが敵の拠点を攻撃！"));
		}
	}

	// もし「攻撃側が敵」かつ「プレイヤーの拠点（Y=-1側）のマスを攻撃範囲に捉えた」場合
	else if (Attacker->PlayerSide == EPlayerSide::Enemy && bHitPlayerBase)
	{
		if (PlayerBase)
		{
			PlayerBase->TakeBuildingDamage(4);
			UE_LOG(LogTemp, Warning, TEXT("EnemyCanAttackPlayerBase"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("拠点攻撃不能"));
	}
}

//移動範囲削除
void AGameManager::DeleteMoveRangeObj() {
	if (CurrentMovePatternObj)
	{
		CurrentMovePatternObj->Destroy();
		CurrentMovePatternObj = nullptr;
	}
}

//フェーズを切り替える
void AGameManager::ChangePhase() {
	DeleteMoveRangeObj();

	switch (currentPhase)
	{
	case CurrentPhase::EGS_Spawn:
		currentPhase = CurrentPhase::EGS_MoveReserve;
		break;

	case CurrentPhase::EGS_MoveReserve:
		currentPhase = CurrentPhase::EGS_Move;
		ExecuteMove();
		break;

	case CurrentPhase::EGS_Move:
		currentPhase = CurrentPhase::EGS_Battle;
		break;

	case CurrentPhase::EGS_Battle:
		currentPhase = CurrentPhase::EGS_Spawn;
		break;

	default:
		break;
	}
}


void AGameManager::DeselectAllCards() {
	for (ACard* Card : AllCardList)
	{
		if (IsValid(Card))
		{
			Card->SetCardSelected(false); // 全員沈める
		}
	}
}


void AGameManager::CheckEndAndShowResult()
{

	// 勝者が決まっていない場合は何もしない
	if (winner == EWinner::None) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	TSubclassOf<UUserWidget> TargetWidgetClass = nullptr;

	// 勝利か敗北かで使う設計図を切り替える
	if (winner == EWinner::Player)
	{
		TargetWidgetClass = WinWidgetClass;
	}
	else if(winner == EWinner::Enemy)
	{
		TargetWidgetClass = LoseWidgetClass;
	}
	else {
		return;
	}

	// 選ばれたウィジェットを生成して画面に出す
	if (TargetWidgetClass)
	{
		UUserWidget* ResultWidget = CreateWidget<UUserWidget>(PC, TargetWidgetClass);
		if (ResultWidget)
		{
			ResultWidget->AddToViewport();

			// マウスカーソルを表示して操作可能にする
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());

			// ゲームを一時停止
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}
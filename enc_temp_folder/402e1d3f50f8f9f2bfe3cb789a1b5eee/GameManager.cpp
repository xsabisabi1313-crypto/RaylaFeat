// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "UnitSpawn.h"
#include "Card.h" // カードのヘッダーを読み込む

#include "Blueprint/UserWidget.h"       // CreateWidget や UUserWidget を使うため
#include "Kismet/GameplayStatics.h"     // UGameplayStatics::SetGamePaused を使うため
#include "GameFramework/PlayerController.h" // APlayerController を使うため（もしエラーが出る場合）
#include "BoardManager.h"
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();


	if (ArrowActorClass)
	{
		FActorSpawnParameters SpawnParams;
		ArrowActor = GetWorld()->SpawnActor<AActor>(ArrowActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (ArrowActor)
		{
			// 最初は非表示にしておく
			ArrowActor->SetActorHiddenInGame(true);
			ArrowActor->SetActorEnableCollision(false); // 当たり判定も消しておく
		}
	}
	
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	AUnit* CurrentHoveredUnit = nullptr;
	if (HitResult.bBlockingHit) {
		CurrentHoveredUnit = Cast<AUnit>(HitResult.GetActor());
	}

	// ==========================================
	// ①「ホバーしているユニットが切り替わった瞬間」の処理
	// ==========================================
	if (CurrentHoveredUnit != HoveredUnit) {
		HoveredUnit = CurrentHoveredUnit;

		if (HoveredUnit) {
			UE_LOG(LogTemp, Warning, TEXT("hoverStart"));

			// 1. ウィジェットがなければ作成する
			if (!UnitTooltipWidget && UnitTooltipWidgetClass)
			{
				UnitTooltipWidget = CreateWidget<UUserWidget>(GetWorld(), UnitTooltipWidgetClass);
				if (UnitTooltipWidget)
				{
					UnitTooltipWidget->AddToViewport();
				}
			}

			// 2. ウィジェットにデータをセットして表示状態にする（※この1回だけ！）
			if (UnitTooltipWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("SetUnitDataToText"));
				struct {
					int32 Power;
					int32 Cost;
					FString Name;
					FString Element;
					FString Ability;
				

				} Params;
				Params.Power = HoveredUnit->Power;
				Params.Cost = HoveredUnit->Cost;
				Params.Name = HoveredUnit->UnitName;
				Params.Ability = HoveredUnit->Ability;
				switch (HoveredUnit->Element)
				{
				case EElementtype::Fire:
					Params.Element = TEXT("炎");
					break;
				case EElementtype::Water:
					Params.Element = TEXT("水");
					break;
				case EElementtype::Grass:
					Params.Element = TEXT("草");
					break;
				default:
					Params.Element = TEXT("無属性");
					break;
				}


				// ウィジェット側の関数を呼び出してパワーを渡す
				UnitTooltipWidget->ProcessEvent(UnitTooltipWidget->FindFunction(FName("SetTooltipData")), &Params);

				//★ここで例外スロー。アクセス違反らしい
				UnitTooltipWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("hoverEnd"));

			// マウスが何も無いところに離れたら非表示にする
			if (UnitTooltipWidget)
			{
				UnitTooltipWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	// ==========================================
	// ②「毎フレーム行うべき処理」：位置の追従
	// ==========================================
	// ホバー中のユニットがいて、かつウィジェットが表示中のときだけ位置を毎フレーム更新する
	if (HoveredUnit && UnitTooltipWidget && UnitTooltipWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		FVector WorldPosition = HoveredUnit->GetActorLocation() + FVector(0.f, 0.f, 100.f);
		FVector2D ScreenPosition;

		bool bIsOnScreen = PC->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition, false);
		if (bIsOnScreen)
		{
			// 毎フレーム、画面上の追従位置を更新する
			UnitTooltipWidget->SetPositionInViewport(ScreenPosition + FVector2D(-500.0f, 15.f), false);
		}
	}
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

	TArray<FIntPoint> AttackPoss = Attacker->GetAvailableAttackPoss();

	// 【ループ①】攻撃できるマスの数だけ回す
	for (const FIntPoint& AttackPos : AttackPoss)
	{
		// 【ループ②】フィールドにいるすべてのキャラクターの数だけ回す
		for (AUnit* Actor : AllUnitsList)
		{
			if (!IsValid(Actor)) continue;

			AUnit* OtherUnit = Actor;
			if (!OtherUnit || OtherUnit == Attacker) continue;

			// 自分と同じ陣営（味方同士）なら攻撃対象外
			if (OtherUnit->PlayerSide == Attacker->PlayerSide) continue;

			// 【判定】「攻撃できるマス」と「キャラの現在地」が一致するか！？
			if ((int32)OtherUnit->GridPos.X == (int32)AttackPos.X && (int32)OtherUnit->GridPos.Y == (int32)AttackPos.Y)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ExecuteBattle] 敵味方衝突！ 相手を発見: GridPos(X:%d, Y:%d), Power:%d"), (int32)OtherUnit->GridPos.X, (int32)OtherUnit->GridPos.Y, (int32)OtherUnit->Power);
				GEngine->AddOnScreenDebugMessage(
					-1,
					3.0f,
					FColor::Orange,
					TEXT("Battle！")
				);

				int32 AttackerPower = (int32)Attacker->Power;
				int32 OtherPower = (int32)OtherUnit->Power;

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
			UE_LOG(LogTemp, Warning, TEXT("[ExecuteBattle] ユニットを破壊します: %s"), *UnitToDestroy->GetName());
			AllUnitsList.RemoveSingle(UnitToDestroy);
			UnitToDestroy->Destroy();
		}
	}

	//もう戦ったなら、終了
	if (isFought) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy Not battle yet "));

	// まだ戦っていないなら、拠点が攻撃可能かどうか判定する。
	bool bHitEnemyBase = false;
	if (IsValid(SelectedUnit)) {
		bHitEnemyBase = SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(0, 0)) ||
			SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(1, 0)) ||
			SelectedUnit->GetAvailableAttackPoss().Contains(FIntPoint(2, 0));

		
	}
	//敵がプレイヤー拠点を攻撃可能かどうか
	bool bHitPlayerBase = false;
	if (IsValid(SelectedEnemyUnit)) {
		bHitPlayerBase = SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(0, 6)) ||
			SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(1, 6)) ||
			SelectedEnemyUnit->GetAvailableAttackPoss().Contains(FIntPoint(2, 6));

		UE_LOG(LogTemp, Warning, TEXT("EnemyBattleBoolTrue"));
	}

	// --- 判定とダメージ処理 ---

	if (Attacker->PlayerSide == EPlayerSide::Player && bHitEnemyBase)
	{
		if (EnemyBase)
		{
			EnemyBase->TakeBuildingDamage(4);
			UE_LOG(LogTemp, Warning, TEXT("プレイヤーが敵の拠点を攻撃！"));
		}
	}
	else if (Attacker->PlayerSide == EPlayerSide::Enemy && bHitPlayerBase)
	{
		if (PlayerBase)
		{
			PlayerBase->TakeBuildingDamage(4);
			UE_LOG(LogTemp, Warning, TEXT("EnemyAttackPlayerBase"));
		}

	}
}


//フェーズを切り替える
void AGameManager::ChangePhase() {

	DisplayMoveReserveArrow(false);
	ABoardManager* BoardManager = Cast<ABoardManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ABoardManager::StaticClass())
	);
	BoardManager->ShowMovableRange(TArray<FIntPoint>());

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
		PlayerCurrentCost += 2;
		EnemyCurrentCost += 2;
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
			Card->isSelectedCard = false;
			Card->SetCardSelected();//⇑の処理のおかげで絶対に沈ませることが可能
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

	if (ResultWidgetClass) {
		CreateWidget<UUserWidget>(PC, ResultWidgetClass)->AddToViewport();
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

void AGameManager::DisplayMoveReserveArrow(bool isActive)
{
	if (!ArrowActor || !SelectedUnit)return;

	if (currentPhase != CurrentPhase::EGS_MoveReserve)return;
	
		// 1. 目的地の座標に移動させる（少し地面から浮かげると綺麗です）
		ArrowActor->SetActorLocation(FVector(ReserveGridPos.X * 100, ReserveGridPos.Y * 100, 20.f));

		// 2. 表示・非表示状態にする
		ArrowActor->SetActorHiddenInGame(!isActive);

}
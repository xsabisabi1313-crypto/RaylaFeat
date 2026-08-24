//このクラスは、プレイヤー側のユニットを召喚するための専用クラス。敵を召喚する処理はEmenyAIクラスにある。
#include "UnitSpawn.h"
#include "Unit.h"
#include "GameManager.h" // GameManagerを使えるようにする
#include "SoundManager.h"
#include "Kismet/GameplayStatics.h" // GetActorOfClassを使うため
// コンストラクタ（最初のお仕事）
AUnitSpawn::AUnitSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

// 味方ユニットをスポーンさせる関数
void AUnitSpawn::SpawnMyUnit(FVector SpawnLocation, FIntPoint SpawnGridPos)
{
	//GameManagerを取得
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

	if (!MyGameManager)return;

	//もしSpawnPhaseでなかったら終わり
	if (MyGameManager->currentPhase != CurrentPhase::EGS_Spawn) {
		return;
	}
	//もしスポーン可能な範囲でなければ終わり
	if (SpawnGridPos.Y != 5 && SpawnGridPos.Y != 6)return;

	//GameManagerの、現在選択されている味方ユニットを選択
	TSubclassOf<AUnit> UnitToSpawn = MyGameManager->UnitClassToSpawn;

    //もし選択されていなかったら終わり
	if (!UnitToSpawn)return;

	//もしコスト的に難しかったら終わり
	AUnit* DefaultUnit = UnitToSpawn.GetDefaultObject();
	if (!DefaultUnit) return;
	if (DefaultUnit->Cost > MyGameManager->PlayerCurrentCost)return;

  //スポーンが確定後
		//	
	//スポーンする(Cardもその位置へ)
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(UnitToSpawn, SpawnLocation, FRotator::ZeroRotator);
	if (CurrentSelectedCard) {
		CurrentSelectedCard->SetActorLocation(SpawnLocation);
	}

	//音を鳴らす
	ASoundManager* SoundMgr = Cast<ASoundManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASoundManager::StaticClass())
	);
	if (SoundMgr) {
		SoundMgr->PlaySE(SoundMgr->SE_UnitSpawn);
	}

	// 初期値を設定
	AUnit* NewUnit = Cast<AUnit>(SpawnedActor);
	if (NewUnit)
	{
		NewUnit->GridPos = SpawnGridPos; // ここに設定したい座標を入れる
		NewUnit->PlayerSide = EPlayerSide::Player;
	}
	//手持ちコストを減らす
	MyGameManager->PlayerCurrentCost -= DefaultUnit->Cost;



	//GameManagerの全キャラリストに追加する
	MyGameManager->AllUnitsList.Add(SpawnedActor);

	return;

}


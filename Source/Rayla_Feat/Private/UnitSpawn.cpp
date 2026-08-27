//このクラスは、プレイヤー側のユニットを召喚するための専用クラス。敵を召喚する処理はEmenyAIクラスにある。
//UnitSpawnではなく、UseCardというクラス名に変えたい
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

// 味方ユニットをスポーン(またはスペル使用)させる関数
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


	//手持ちコストを減らす(共通)
	MyGameManager->PlayerCurrentCost -= DefaultUnit->Cost;


	//(Cardは非表示に)(共通)
	if (CurrentSelectedCard) {
		CurrentSelectedCard->SetActorLocation(FVector(1000, 1000, 1000));
	}

	//選択しているユニットをnullにし、同じユニットを召喚できないように(共通)
	//UnitToSpawn = nullptr;
	MyGameManager->UnitClassToSpawn = nullptr;

	//音を鳴らす(共通)
	ASoundManager* SoundMgr = Cast<ASoundManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASoundManager::StaticClass())
	);
	if (SoundMgr) {
		SoundMgr->PlaySE(SoundMgr->SE_UnitSpawn);
	}



	switch (CurrentSelectedCard->CardType) {

	case ECardType::UnitCard:
	{
		//スポーンする(味方ユニットなので、180度回転させる)(unit)
		FRotator SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
		AUnit* SpawnedActor = GetWorld()->SpawnActor<AUnit>(UnitToSpawn, SpawnLocation, SpawnRotation);

		// 初期値を設定(unit)
		if (SpawnedActor)
		{
			SpawnedActor->GridPos = SpawnGridPos; // ここに設定したい座標を入れる
			SpawnedActor->PlayerSide = EPlayerSide::Player;
		}

		//GameManagerの全キャラリストに追加する(unit)
		MyGameManager->AllUnitsList.Add(SpawnedActor);

		break;
	}

	case ECardType::Spell_Kibidango: 
	{
		for (AUnit* Unit : MyGameManager->AllUnitsList)
		{
			if (Unit->UnitTeam != EUnitTeam::Momotaro)return;

			Unit->Power += 2;
		}
		break;

	}

	}



	return;

}


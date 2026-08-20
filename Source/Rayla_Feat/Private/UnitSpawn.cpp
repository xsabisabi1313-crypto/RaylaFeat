
#include "UnitSpawn.h"
#include "GameManager.h" // GameManagerを使えるようにする
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

	//もしSpawnPhaseでなかったら終わり
	if (MyGameManager->currentPhase != CurrentPhase::EGS_Spawn) {
		return;
	}

	//GameManagerの、現在選択されている味方ユニットを選択
	TSubclassOf<AActor> UnitToSpawn = MyGameManager->UnitClassToSpawn;

    

	//スポーンさせて、スポーンさせたユニットをreturnで返す
	if (UnitToSpawn)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;

		//コスト的にできるか

		//スポーンが確定した後
		//	
		// 1. スポーンする（最初は AActor* として受け取る）
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(UnitToSpawn, SpawnLocation, SpawnRotation);

		// 2. AUnit に変換（キャスト）する
		AUnit* NewUnit = Cast<AUnit>(SpawnedActor);

		// 3. キャストが成功したら GridPos をセットする
		if (NewUnit)
		{
			NewUnit->GridPos = SpawnGridPos; // ここに設定したい座標を入れる
			NewUnit->PlayerSide = EPlayerSide::Player;
		}

		

		//4. コストを減らす

		//5.GameManagerの全キャラリストに追加する
		MyGameManager->AllUnitsList.Add(SpawnedActor);



		return;
	}

	//生成失敗した場合
	return;

}

//敵ユニットをスポーンさせる関数
void AUnitSpawn::SpawnEnemyUnit() {

	//GameManagerを取得
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

	if (!MyGameManager) return;

	// 2. 敵の設計書リスト（EnemyOwnedUnits）が空っぽじゃないかチェック
	if (MyGameManager->EnemyOwnedUnits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("敵のスポーンリストが空です！"));
		return;
	}
	// 3. どの敵をスポーンさせるか決める
	// （例：とりあえずリストの0番目の敵クラスを選ぶ。
	int32 IndexToSpawn = 0;
	TSubclassOf<AUnit> EnemyClassToSpawn = MyGameManager->EnemyOwnedUnits[IndexToSpawn];
	if (!EnemyClassToSpawn) return ;

	// 4. スポーンさせる場所を決める
	FVector SpawnLocation = FVector::ZeroVector; 
	FIntPoint SpawnGridPos = FIntPoint(0, 0);  
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 5. 実際にワールドにスポーンさせる
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn, SpawnLocation, SpawnRotation);

	AUnit* NewUnit = Cast<AUnit>(SpawnedActor);
	if (NewUnit)
	{
		// 6. グリッド位置と「敵陣営（Enemy）」であることをセットする
		NewUnit->GridPos = SpawnGridPos;
		NewUnit->PlayerSide = EPlayerSide::Enemy; 
	}

	// 7. GameManagerの全キャラリストに追加する
	MyGameManager->AllUnitsList.Add(SpawnedActor);

}

#include "UnitSpawn.h"
#include "GameManager.h" // GameManagerを使えるようにする
#include "Kismet/GameplayStatics.h" // GetActorOfClassを使うため
// コンストラクタ（最初のお仕事）
AUnitSpawn::AUnitSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ユニットをスポーンさせる関数
AActor* AUnitSpawn::SpawnMyUnit(FVector SpawnLocation, FIntPoint SpawnGridPos)
{
	//GameManagerを取得
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);
	//GameManagerの、現在選択されている味方ユニットを選択
	TSubclassOf<AActor> UnitToSpawn = MyGameManager->UnitClassToSpawn;
    

	//GetWorld....でスポーンさせて、スポーンさせたユニットをreturnで返す
	if (UnitToSpawn)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 1. スポーンする（最初は AActor* として受け取る）
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(UnitToSpawn, SpawnLocation, SpawnRotation);

		// 2. AUnit に変換（キャスト）する
		AUnit* NewUnit = Cast<AUnit>(SpawnedActor);

		// 3. キャストが成功したら GridPos をセットする
		if (NewUnit)
		{
			NewUnit->GridPos = SpawnGridPos; // ここに設定したい座標を入れる
		}

		return SpawnedActor;
	}

	//生成失敗した場合
	return nullptr;

}
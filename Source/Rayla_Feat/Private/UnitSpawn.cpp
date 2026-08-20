
#include "UnitSpawn.h"
#include "GameManager.h" // GameManagerを使えるようにする
#include "Kismet/GameplayStatics.h" // GetActorOfClassを使うため
// コンストラクタ（最初のお仕事）
AUnitSpawn::AUnitSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ユニットをスポーンさせる関数
AActor* AUnitSpawn::SpawnMyUnit(FVector SpawnLocation)
{
	//GameManagerを取得
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);
	//GameManagerの、現在選択されている味方ユニットを選択
	TSubclassOf<AActor> UnitToSpawn = MyGameManager->SelectedUnit;
    

	//GetWorld....でスポーンさせて、スポーンさせたユニットをreturnで返す
	if (UnitToSpawn)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		return GetWorld()->SpawnActor<AActor>(UnitToSpawn, SpawnLocation, SpawnRotation);
	}

	//生成失敗した場合
	return nullptr;

}
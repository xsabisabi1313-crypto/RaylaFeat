
#include "UnitSpawn.h"

// コンストラクタ（最初のお仕事）
AUnitSpawn::AUnitSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ユニットをスポーンさせる関数
AActor* AUnitSpawn::SpawnMyUnit(TSubclassOf<AActor> UnitClassToSpawn, FVector SpawnLocation)
{

	//GetWorld....でスポーンさせて、スポーンさせたユニットをreturnで返す
	if (UnitClassToSpawn)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		return	GetWorld()->SpawnActor<AActor>(UnitClassToSpawn, SpawnLocation, SpawnRotation);
	}

	//生成失敗した場合
	return nullptr;

}
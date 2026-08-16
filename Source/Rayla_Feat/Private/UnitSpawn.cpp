
#include "UnitSpawn.h"

// コンストラクタ（最初のお仕事）
AUnitSpawn::AUnitSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ユニットをスポーンさせる関数
void AUnitSpawn::SpawnMyUnit(TSubclassOf<AActor> UnitClassToSpawn, FVector SpawnLocation)
{

	if (UnitClassToSpawn)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AActor>(UnitClassToSpawn, SpawnLocation, SpawnRotation);
	}
}
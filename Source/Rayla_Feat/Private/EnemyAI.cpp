// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include"GameManager.h"


// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEnemyAI::ExecuteAITurn() {
	if (GameManagerRef->currentPhase == CurrentPhase::EGS_Spawn) {
		ProcessAISpawn();
	}
	else if (GameManagerRef->currentPhase == CurrentPhase::EGS_MoveReserve) {
		ProcessAIMoveReserve();
	}

}

void AEnemyAI::ProcessAISpawn()
{

    FIntPoint ChosenPos;

    // どのキャラをスポーンさせるか決める
    int32 IndexToSpawn = 0;
    TSubclassOf<AUnit> EnemyClassToSpawn = GameManagerRef->EnemyOwnedUnits[IndexToSpawn];
    if (!EnemyClassToSpawn) return;

    //召喚する場所を決める
    //まず、召喚できる場所を初期6マスある。
    TArray<FIntPoint> SpawnableCandidatePositions = {
        FIntPoint(0, 0), FIntPoint(1, 0), FIntPoint(2, 0),
        FIntPoint(0, 1), FIntPoint(1, 1), FIntPoint(2, 1)
    };

    // もし「召喚候補の中に、すでにユニットがいる座標」が含まれていたら、候補から外す！
    for (AActor* Actor : GameManagerRef->AllUnitsList)
    {
        AUnit* Unit = Cast<AUnit>(Actor);
        if (!Unit) continue;
        FIntPoint UnitPos = Unit->GridPos;
        
        SpawnableCandidatePositions.Remove(UnitPos);
    }

    // 残った候補の中から、ランダムに1つ選ぶ！
    if (SpawnableCandidatePositions.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, SpawnableCandidatePositions.Num() - 1);
        ChosenPos = SpawnableCandidatePositions[RandomIndex];

    }

    // 5. 実際にワールドにスポーンさせる
    AUnit* SpawnedActor = GetWorld()->SpawnActor<AUnit>(EnemyClassToSpawn, FVector(ChosenPos.X*100, ChosenPos.Y*100,0.0f), FRotator::ZeroRotator);

    // 2. AUnit に変換（キャスト）する
    AUnit* NewUnit = Cast<AUnit>(SpawnedActor);

    // 3. キャストが成功したら GridPos をセットする
    if (NewUnit)
    {
        NewUnit->GridPos = ChosenPos; // ここに設定したい座標を入れる
        NewUnit->PlayerSide = EPlayerSide::Player;
    }

    //4. コストを減らす

    //5.GameManagerの全キャラリストに追加する
    GameManagerRef->AllUnitsList.Add(SpawnedActor);


}
void AEnemyAI::ProcessAIMoveReserve() {

}

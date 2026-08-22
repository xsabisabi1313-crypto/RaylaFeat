// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include"GameManager.h"


// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEnemyAI::ExecuteAITurn() {

    if (!GameManagerRef)return;

	if (GameManagerRef->currentPhase == CurrentPhase::EGS_Spawn) {
		ProcessAISpawn();
	}
	else if (GameManagerRef->currentPhase == CurrentPhase::EGS_MoveReserve) {
		ProcessAIMoveReserve();
	}

}

void AEnemyAI::ProcessAISpawn()
{

    if(!GameManagerRef) return;

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
        if (!IsValid(Actor)) continue;
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
        NewUnit->PlayerSide = EPlayerSide::Enemy;
    }

    //4. コストを減らす

    //5.GameManagerの全キャラリストに追加する
    GameManagerRef->AllUnitsList.Add(SpawnedActor);


}
void AEnemyAI::ProcessAIMoveReserve() {

    if (!GameManagerRef) return;
    //まず、移動するキャラクターを決める
    //全てのユニットの中から敵だけを候補にし、どれか一つをランダムに決定する

    // 1. その場で敵だけを抽出するリストを作る
    TArray<AUnit*> EnemyUnits;
    for (AActor* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Cast<AUnit>(Actor);
        if (Unit && Unit->PlayerSide == EPlayerSide::Enemy) // 陣営に合わせて変更してください
        {
            EnemyUnits.Add(Unit);
        }
    }



    if (EnemyUnits.Num() == 0) return;

    // 2. ランダムに1体選ぶ
    int32 RandomIndex = FMath::RandRange(0, EnemyUnits.Num() - 1);
    AUnit* ChosenUnitToMove = EnemyUnits[RandomIndex];
    GameManagerRef->SelectedEnemyUnit = ChosenUnitToMove;



    if (ChosenUnitToMove)
    {
        // 3. 現在地を基準に「前方のマス」を計算する
        //基本は前(Y座標プラス方向)に向かう
        FIntPoint CurrentPos = ChosenUnitToMove->GridPos;
        FIntPoint TargetPos = FIntPoint(CurrentPos.X, CurrentPos.Y + 1); // 前方（Yプラス方向）

        // 4. 計算した移動予定地をGameManagerに保持させる
        GameManagerRef->ReserveEnemyGridPos = TargetPos;

        GEngine->AddOnScreenDebugMessage(
            -1, 3.0f, FColor::Green,
            FString::Printf(TEXT("Enemy decided to move to X:%d, Y:%d"), TargetPos.X, TargetPos.Y)
        );
    }
    

}

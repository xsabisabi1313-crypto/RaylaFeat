// ここでやっていることは、あくまで召喚する処理と移動する場所の決定だけ。


#include "EnemyAI.h"
#include"GameManager.h"


// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}

//状況に応じて何かしらやってくれる、一つの窓口（いつ呼んでもエラー起きないはず）
void AEnemyAI::ExecuteAITurn() {

    if (!GameManagerRef)return;

	if (GameManagerRef->currentPhase == CurrentPhase::EGS_Spawn) {
		ProcessAISpawn();
	}
	else if (GameManagerRef->currentPhase == CurrentPhase::EGS_MoveReserve) {
		ProcessAIMoveReserve();
	}

}

//召喚フェーズで敵を召喚する関数
void AEnemyAI::ProcessAISpawn()
{

    if(!GameManagerRef) return;

    FIntPoint ChosenPos;

    //１，何のキャラをスポーンさせるかを決める
    //  すでに場にいる（生存している）すべての「敵ユニットのクラス」を集める
    TArray<TSubclassOf<AUnit>> SpawnedEnemyClasses;
    for (AActor* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Cast<AUnit>(Actor);
        if (Unit && Unit->PlayerSide == EPlayerSide::Enemy)
        {
            SpawnedEnemyClasses.Add(Unit->GetClass());
        }
    }

    //  敵が所持しているリストから、「まだ場に出ていないキャラ」だけを候補として集める
    TArray<TSubclassOf<AUnit>> AvailableClasses;
    for (TSubclassOf<AUnit> OwnedClass : GameManagerRef->EnemyOwnedUnits)
    {
        if (!OwnedClass) continue;

        // まだ場に出ていなければ候補に追加
        if (!SpawnedEnemyClasses.Contains(OwnedClass))
        {
            AvailableClasses.Add(OwnedClass);
        }
    }




    // もし「まだ場に出ていないキャラ」が1体もいなければ、これ以上召喚できないので終了
    if (AvailableClasses.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("すべての種類の敵がすでに場に出現しています！"));
        return;
    }
    //召喚させるユニット決定！
    int32 RandomIndex = FMath::RandRange(0, AvailableClasses.Num() - 1);
    TSubclassOf<AUnit> EnemyClassToSpawn = AvailableClasses[RandomIndex];


    //2、召喚する場所を決める
    //まず、召喚できる場所は、初期6マスある。
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
        RandomIndex = FMath::RandRange(0, SpawnableCandidatePositions.Num() - 1);
        ChosenPos = SpawnableCandidatePositions[RandomIndex];

    }

    // 3、 実際にワールドにスポーンさせる
    AUnit* SpawnedActor = GetWorld()->SpawnActor<AUnit>(EnemyClassToSpawn, FVector(ChosenPos.X*100, ChosenPos.Y*100,0.0f), FRotator::ZeroRotator);

    // 4,ユニットの初期設定やらなんやら
    AUnit* NewUnit = Cast<AUnit>(SpawnedActor);
    if (NewUnit)
    {
        NewUnit->GridPos = ChosenPos; 
        NewUnit->PlayerSide = EPlayerSide::Enemy;

        // コストを減らす
        GameManagerRef->EnemyCurrentCost -= NewUnit->Cost;

        //GameManagerの全キャラリストに追加する
        GameManagerRef->AllUnitsList.Add(SpawnedActor);
    }




}

//移動する場所を決める関数
void AEnemyAI::ProcessAIMoveReserve() {



    if (!GameManagerRef) return;
    UE_LOG(LogTemp, Warning, TEXT("移動する場所を決めます"));
    //まず、移動するキャラクターを決める
    //全てのユニットの中から敵だけを候補にし、どれか一つをランダムに決定する

    // 1. その場で敵だけを抽出するリストを作る
    TArray<AUnit*> EnemyUnits;
    for (AActor* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Cast<AUnit>(Actor);
        if (Unit && Unit->PlayerSide == EPlayerSide::Enemy)
        {
            EnemyUnits.Add(Unit);
        }
    }



    if (EnemyUnits.Num() == 0) return;
    UE_LOG(LogTemp, Warning, TEXT("今からランダムに選ぶぜ"));
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
        UE_LOG(LogTemp, Warning, TEXT("移動する場所決定！"));

        GEngine->AddOnScreenDebugMessage(
            -1, 3.0f, FColor::Green,
            FString::Printf(TEXT("Enemy decided to move to X:%d, Y:%d"), TargetPos.X, TargetPos.Y)
        );
    }
    

}

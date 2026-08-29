// ここでやっていることは、あくまで召喚する処理と移動する場所の決定だけ。


#include "EnemyAI.h"
#include"Card.h"
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

//召喚フェーズで敵を召喚、またはカード使用する関数
void AEnemyAI::ProcessAISpawn()
{

    if(!GameManagerRef) return;

    FIntPoint ChosenPos = FIntPoint(0,0);

    //１，何のキャラをスポーンさせるかを決める
    //  すでに場にいる（生存している）すべての「敵ユニットのクラス」を集める
    TArray<TSubclassOf<AUnit>> SpawnedEnemyClasses;
    for (AUnit* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Actor;
        if (Unit && Unit->PlayerSide == EPlayerSide::Enemy)
        {
            SpawnedEnemyClasses.Add(Unit->GetClass());
        }
    }

    //spawnCountが3つなら、スペルを使用
    if (spawnCount >= 3) {
        UseSpell();
        return;
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

    //だが、コスト的に行けるか確認し、行けなかったら何もせず終了
    if (GameManagerRef->EnemyCurrentCost < EnemyClassToSpawn.GetDefaultObject()->Cost) return;


    //2、召喚する場所を決める
    //まず、召喚できる場所は、初期3マスある。
    TArray<FIntPoint> SpawnableCandidatePositions = {
        
        FIntPoint(0, 1), FIntPoint(1, 1), FIntPoint(2, 1)
    };

    // もし「召喚候補の中に、すでにユニットがいる座標」が含まれていたら、候補から外す！
    for (AUnit* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Actor;
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

        spawnCount++;
    }

    //手持ちのカードを非表示に
    for (ACard* card:GameManagerRef->EnemyCardList) {
        if (card->UnitToSpawn == EnemyClassToSpawn) {
            card->SetActorLocation(FVector(-1000, -1000, -1000));
            break;
        }
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
    for (AUnit* Actor : GameManagerRef->AllUnitsList)
    {
        if (!IsValid(Actor)) continue;
        AUnit* Unit = Actor;
        if (Unit && Unit->PlayerSide == EPlayerSide::Enemy)
        {
            EnemyUnits.Add(Unit);
        }
    }
    if (EnemyUnits.Num() == 0) return;


    // 2. ランダムに1体選ぶ
    int32 RandomIndex = FMath::RandRange(0, EnemyUnits.Num() - 1);
    AUnit* ChosenUnitToMove = EnemyUnits[RandomIndex];
    GameManagerRef->SelectedEnemyUnit = ChosenUnitToMove;
    if (!ChosenUnitToMove)return;



    // もしすでに拠点を攻撃できる位置にいるなら、前に進まずにその場にとどまる
    // プレイヤーの拠点座標（(0,7), (1,7), (2,7) など）のいずれかが、自分の攻撃可能マスに含まれているか判定
    TArray<FIntPoint> AttackPoss = ChosenUnitToMove->GetAvailableAttackPoss();
    
    bool bCanAttackPlayerBase = AttackPoss.Contains(FIntPoint(0, 6)) ||
        AttackPoss.Contains(FIntPoint(1, 6)) ||
        AttackPoss.Contains(FIntPoint(2, 6));
    if (bCanAttackPlayerBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("拠点が攻撃範囲内にあるため、移動せずにとどまります！"));
        GameManagerRef->ReserveEnemyGridPos = ChosenUnitToMove->GridPos; // 移動先を「現在地」にする
        return;
    }


    //そのユニットの移動可能範囲を調べる
    //もし移動可能範囲が０なら動かない。後々やり直ししたい（再帰的にProcessAIMoveReserve()を呼びたい）が、無限loopになる可能性があるため保留
    //基本は前に動く

    TArray<FIntPoint> MovePoss = ChosenUnitToMove->GetAvailableMovePoss();
    FIntPoint TargetPos = FIntPoint(ChosenUnitToMove->GridPos.X, ChosenUnitToMove->GridPos.Y + 1); // 前方（Yプラス方向）
    // 前方が移動可能範囲に含まれているか判定する
    bool bCanMoveToTarget = MovePoss.Contains(TargetPos);


    if (!bCanMoveToTarget) {
        GameManagerRef->ReserveEnemyGridPos = ChosenUnitToMove->GridPos;
    }
    else {
        GameManagerRef->ReserveEnemyGridPos = TargetPos;
    }
    

    UE_LOG(LogTemp, Warning, TEXT("移動する場所決定！"));
    
    

}

void AEnemyAI::UseSpell() {
    TArray<ACard*> EnemyAllSpellCard = GameManagerRef->EnemyCardList;

    UE_LOG(LogTemp, Warning, TEXT("cardokiiAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));

    for (ACard* card : EnemyAllSpellCard)
    {
        if (!IsValid(card)) continue;

        if (card->CardType == ECardType::Spell_RedWine) {

            UE_LOG(LogTemp, Warning, TEXT("Wine使うokiiAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
            AUnit* SpawnedActor = GetWorld()->SpawnActor<AUnit>(ObaachanUnit, FVector(100, 100, 0.0f), FRotator::ZeroRotator);
            AUnit* NewUnit = Cast<AUnit>(SpawnedActor);
            if (NewUnit)
            {
                NewUnit->GridPos = FIntPoint(1, 1);
                NewUnit->PlayerSide = EPlayerSide::Enemy;
                GameManagerRef->EnemyCurrentCost -= NewUnit->Cost;
                GameManagerRef->AllUnitsList.Add(SpawnedActor);
                spawnCount++;
            }

            // カードを移動させる
            card->SetActorLocation(FVector(-1000, -1000, -1000));

            // リストからの削除はカードを動かした「後」に安全に行う
            GameManagerRef->EnemyCardList.Remove(card);
            return;
        }
        else if (card->CardType == ECardType::Spell_Ohanatumi) {

            for (AUnit* Actor : GameManagerRef->AllUnitsList)
            {
                if (!IsValid(Actor)) continue;
                if (Actor->PlayerSide == EPlayerSide::Enemy)
                {
                    Actor->Power += 1;
                }
            }

            UE_LOG(LogTemp, Warning, TEXT("OhanaokiiAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
            card->SetActorLocation(FVector(-1000, -1000, -1000));
            GameManagerRef->EnemyCardList.Remove(card);
            return;
        }
        else if (card->CardType == ECardType::Spell_Ookiinone) {


            UE_LOG(LogTemp, Warning, TEXT("okiiAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
            for (AUnit* Actor : GameManagerRef->AllUnitsList)
            {
                if (!IsValid(Actor)) continue;
                if (Actor->PlayerSide == EPlayerSide::Enemy)
                {
                    Actor->Power += 2;
                }
            }

            card->SetActorLocation(FVector(-1000, -1000, -1000));
            GameManagerRef->EnemyCardList.Remove(card);
            return;
        }
    }
}
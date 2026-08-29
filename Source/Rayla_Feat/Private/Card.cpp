#include "Card.h"
#include "GameManager.h" 
#include "UnitSpawn.h"
#include "SoundManager.h"
#include "BoardManager.h"
#include "Kismet/GameplayStatics.h" // GameManagerを探すために必要

// Sets default values
ACard::ACard()
{
	PrimaryActorTick.bCanEverTick = true;

	bEnableAutoLODGeneration = true;
}


// 実際にクリックされたときの処理
void ACard::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

    AGameManager* MyGameManager = Cast<AGameManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
    );
    ASoundManager* SoundMgr = Cast<ASoundManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASoundManager::StaticClass())
    );
    AUnitSpawn* UnitSpawner = Cast<AUnitSpawn>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AUnitSpawn::StaticClass())
    );

    ABoardManager* BoardMgr = Cast<ABoardManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ABoardManager::StaticClass())
    );

    if (!MyGameManager)return;
    if (!UnitSpawner)return;

    if (SoundMgr) { SoundMgr->PlaySE(SoundMgr->SE_UnitMove); }
    if(MyGameManager->currentPhase != CurrentPhase::EGS_Spawn)return;

    //
	UE_LOG(LogTemp, Warning, TEXT("cardクリックしました: %s"), *GetName());



    UnitSpawner->CurrentSelectedCard = this;
    MyGameManager->UnitClassToSpawn = UnitToSpawn;

    switch (CardType)
    {
    case ECardType::UnitCard:
        
        break;
    case ECardType::Spell_Kibidango:
        break;
    case ECardType::Spell_RedWine:
        break;
    default:
        break;
    }
    
   
    
	
	

    //自分以外の全てのカードを沈ませる
    MyGameManager->DeselectAllCards();
    //そのあと自分だけ挙げる(未選択の場合だけ)
    if (!isSelectedCard) {
        isSelectedCard = true;
        SetCardSelected();
    }

    TArray<FIntPoint> tiles = {FIntPoint(0, 5), FIntPoint(1, 5), FIntPoint(2, 5)};
    BoardMgr->ShowMovableRange(tiles);

}

//見た目：沈ませたり浮かばせる関数
void ACard::SetCardSelected() {

    //クリックされていたなら、挙げる
    if (isSelectedCard) {
        
        FVector CurrentLocation = GetActorLocation();
        SetActorLocation(FVector(CurrentLocation.X,CurrentLocation.Y,100));
    }
    //沈ませる
    else {
        FVector CurrentLocation = GetActorLocation();
        SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, 60));
    }
}
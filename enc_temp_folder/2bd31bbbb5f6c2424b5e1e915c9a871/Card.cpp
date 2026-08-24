#include "Card.h"
#include "GameManager.h" 
#include "UnitSpawn.h"
#include "SoundManager.h"
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
	UE_LOG(LogTemp, Warning, TEXT("cardクリックしました: %s"), *GetName());

    //音を鳴らす
    ASoundManager* SoundMgr = Cast<ASoundManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ASoundManager::StaticClass())
    );
    if (SoundMgr) {
        SoundMgr->PlaySE(SoundMgr->SE_UnitMove);
    }


    
    AUnitSpawn* UnitSpawner = Cast<AUnitSpawn>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AUnitSpawn::StaticClass())
    );
    if (!UnitSpawner)return;
    UnitSpawner->CurrentSelectedCard = this;
    

	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

    if (!MyGameManager)return;
    MyGameManager->UnitClassToSpawn = UnitToSpawn;
	
	

    //自分以外の全てのカードを沈ませる
    MyGameManager->DeselectAllCards();
    //そのあと自分だけ挙げる(未選択の場合だけ)
    if (!isSelectedCard) {
        isSelectedCard = true;
        SetCardSelected();
    }
    

}

//見た目：沈ませたり浮かばせる関数
void ACard::SetCardSelected() {

    //クリックされていたなら、挙げる
    if (isSelectedCard) {
        
        FVector CurrentLocation = GetActorLocation();
        SetActorLocation(FVector(CurrentLocation.X,CurrentLocation.Y,100));
    }
    else {
        FVector CurrentLocation = GetActorLocation();
        SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, -100));
    }
}
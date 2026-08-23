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


    SetCardSelected(true);
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

}

//見た目：沈ませたり浮かばせる関数
void ACard::SetCardSelected(bool IsSelected) {


    isSelectedCard = !isSelectedCard;

    //クリックされていたなら、関係なく沈ませる
    if (isSelectedCard) {
        AGameManager* MyGameManager = Cast<AGameManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
        );

        if (!MyGameManager)return;
        MyGameManager->UnitClassToSpawn = nullptr;
        // 【沈ませる（元に戻す）処理】
        FVector CurrentLocation = GetActorLocation();
        CurrentLocation.Z -= 30.0f;
        SetActorLocation(CurrentLocation);
    }





    // 現在地を取得
    FVector CurrentLocation = GetActorLocation();

    if (IsSelected)
    {
        // 【浮かばせる処理】
        CurrentLocation.Z += 30.0f;
        SetActorLocation(CurrentLocation);
    }
    else
    {
        // 【沈ませる（元に戻す）処理】
        CurrentLocation.Z -= 30.0f;
        SetActorLocation(CurrentLocation);
    }
}
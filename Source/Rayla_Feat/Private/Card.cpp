#include "Card.h"
#include "GameManager.h" 
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

    SetCardSelected(true);

	// 1. ワールドから GameManager を探して取得する
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

	if (MyGameManager)
	{
		MyGameManager->UnitClassToSpawn = UnitToSpawn;
		UE_LOG(LogTemp, Warning, TEXT("GameManagerの SelectedUnit！"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameManagerが見つかりませんでした！"));
	}
}

//見た目：沈ませたり浮かばせる関数
void ACard::SetCardSelected(bool IsSelected) {

    // 現在地を取得
    FVector CurrentLocation = GetActorLocation();

    if (IsSelected)
    {
        // 【浮かばせる処理】
        // 例：Z軸（またはY軸）に少し持ち上げる
        // ※プロジェクトの軸の向きに合わせて数値を調整してください
        CurrentLocation.Z += 30.0f;
        SetActorLocation(CurrentLocation);
    }
    else
    {
        // 【沈ませる（元に戻す）処理】
        // 例：持ち上げた分を戻す（※単純に足し引きするとズレる場合があるので、元の基準位置を持っておくとなお良しです！）
        CurrentLocation.Z -= 30.0f;
        SetActorLocation(CurrentLocation);
    }
}
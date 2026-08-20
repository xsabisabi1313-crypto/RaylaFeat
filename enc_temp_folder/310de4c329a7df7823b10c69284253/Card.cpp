#include "Card.h"
#include "GameManager.h" // インクルードのパスを正しい形に直しました
#include "Kismet/GameplayStatics.h" // GameManagerを探すために必要

// Sets default values
ACard::ACard()
{
	PrimaryActorTick.bCanEverTick = true;

	bEnableAutoLODGeneration = true;
}

void ACard::BeginPlay()
{
	Super::BeginPlay();
}

void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 実際にクリックされたときの処理
void ACard::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("cardクリックしました: %s"), *GetName());

	// 1. ワールドから GameManager を探して取得する
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

	// 2. 見つかったら SelectedUnit を nullptr にする
	if (MyGameManager)
	{
		MyGameManager->SelectedUnit = UnitToSpawn;
		UE_LOG(LogTemp, Warning, TEXT("GameManagerの SelectedUnit をリセット(null)しました！"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameManagerが見つかりませんでした！"));
	}
}
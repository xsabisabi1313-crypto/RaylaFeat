// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GameManager.h"
#include "UnitSpawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 「クリックを検知する」設定を有効にする ▼ ---
	bEnableAutoLODGeneration = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bEnableClickEvents = true;
	}
	
}




void ATile::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) {


	UE_LOG(LogTemp, Warning, TEXT("ClickedTile"));

	AGameManager* GameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);

	AUnitSpawn* Spawner = Cast<AUnitSpawn>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AUnitSpawn::StaticClass())
	);

	if (!GameManager)return;

	if (GameManager->currentPhase == CurrentPhase::EGS_Spawn) {
		Spawner->SpawnMyUnit(FVector(GridXY.X * 100, GridXY.Y * 100,0), GridXY);
	}
	else if (GameManager->currentPhase == CurrentPhase::EGS_MoveReserve) {
		GameManager->ReserveGridPos = GridXY;
		GameManager->DisplayMoveReserveArrow(true);
	}
}

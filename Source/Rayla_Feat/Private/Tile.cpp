// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "GameManager.h"
#include "UnitSpawn.h"
#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	bEnableAutoLODGeneration = true;

	// メッシュコンポーネントの実体を作成
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh;

	// ★ ここでコードから直接スタティックメッシュをロードしてセットする！
	// （※ /Content/ 配下のメッシュのパスを指定します。例としてUEの標準的なCubeを指定しています）
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bEnableClickEvents = true;
	}

	TileMesh->MarkRenderStateDirty();
	TileMesh->UpdateBounds();
	TileMesh->MarkRenderStateDirty();

	FVector CurrentLoc = GetActorLocation();
	SetActorLocation(CurrentLoc + FVector(0.0f, 0.0f, 0.1f));
	
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
		if (GameManager->SelectedUnit->GetAvailableMovePoss().Contains(GridXY)) {
			GameManager->ReserveGridPos = GridXY;
			GameManager->DisplayMoveReserveArrow(true);
		}

	}
}


void ATile::SetHighlight(bool bIsHighlighted)
{
	if (!TileMesh) return;

	if (bIsHighlighted)
	{
		// ハイライト用のマテリアルにすり替える
		if (HighlightMaterial)
		{
			TileMesh->SetMaterial(0, HighlightMaterial);
		}
	}
	else
	{
		// 通常用のマテリアルに戻す
		if (NormalMaterial)
		{
			TileMesh->SetMaterial(0, NormalMaterial);
		}
	}
}
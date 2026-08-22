// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include <GameManager.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- ▼ 重要！「クリックを検知する」設定を有効にする ▼ ---
	bEnableAutoLODGeneration = true;
}


void AUnit::BeginPlay()
{
	Super::BeginPlay();

	// 自分のルートコンポーネント（またはメッシュ・コリジョン）を取得してバインドする
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(RootComponent))
	{
		// マウスが乗ったとき
		PrimComp->OnBeginCursorOver.AddDynamic(this, &AUnit::OnCursorBeginOver);

		// マウスが離れたとき
		PrimComp->OnEndCursorOver.AddDynamic(this, &AUnit::OnCursorEndOver);
	}
}

// ユニットがクリックされたときの処理
void AUnit::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{

	//味方じゃなかったら反応させない
	if (PlayerSide != EPlayerSide::Player) // 敵の場合
	{
		
		return;
	}


	// ワールド上のGameManagerを探して自分をセットする
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);
	if (!MyGameManager)return;
	MyGameManager->SelectedUnit = this;


	//MyGameManager->SelectedUnit = this;
	UE_LOG(LogTemp, Warning, TEXT("ユニットを選択しました: %s"), *GetName());
	if (MyGameManager->currentPhase == CurrentPhase::EGS_MoveReserve) {
		SpawnMovePatternObject();
		MyGameManager->GetAvailableMovePositions(this);
	}
	
}


void AUnit::OnCursorBeginOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("マウスが乗った！: %s"), *GetName());
}

void AUnit::OnCursorEndOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("マウスが離れた！: %s"), *GetName());
}



//現在の位置を更新する
void AUnit::MoveToGrid(FIntPoint NewGridPos)
{

	if (!IsValid(this)) return;

	GridPos = NewGridPos;

	FVector NewWorldLocation = FVector(GridPos.X * 100.0f, GridPos.Y * 100.0f, GetActorLocation().Z);

	// 3. アクターの実際の3D位置を更新する
	SetActorLocation(NewWorldLocation);
}

//移動できる範囲を生成する
void AUnit::SpawnMovePatternObject()
{

	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);


	if (MyGameManager->CurrentMovePatternObj) {
		MyGameManager->DeleteMoveRangeObj();
	}
	MyGameManager->CurrentMovePatternObj = GetWorld()->SpawnActor<AActor>(MovePatternObjClass, FVector(GridPos.X * 100, GridPos.Y * 100, 46), FRotator::ZeroRotator);
}



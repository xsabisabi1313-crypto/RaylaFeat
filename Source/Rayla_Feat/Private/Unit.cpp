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

	// 例として、持っているRootComponent（またはコリジョン/メッシュ）の OnClicked に関数をバインドする
	// ※ 実際にお使いのコリジョンやメッシュの変数名に合わせてください
	if (RootComponent)
	{
		// プリミティブコンポーネント（コリジョンやメッシュなど）のクリックイベントに結びつける
		//if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(RootComponent))
		//{
		//	PrimComp->OnClicked.AddDynamic(this, &ABP_UnitBase::OnMyActorClicked);
		//}
	}
}

// ユニットがクリックされたときの処理
void AUnit::OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("クリックしました: %s"), *GetName());

	// ワールド上のGameManagerを探して自分をセットする
	AGameManager* MyGameManager = Cast<AGameManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
	);
	MyGameManager->SelectedUnit = this;


	//MyGameManager->SelectedUnit = this;
	UE_LOG(LogTemp, Warning, TEXT("ユニットを選択しました: %s"), *GetName());
	if (MyGameManager->currentPhase == CurrentPhase::EGS_MoveReserve) {
		SpawnMovePatternObject();
		MyGameManager->GetAvailableMovePositions(this);
	}
	
}



//現在の位置を更新する
void AUnit::MoveToGrid(FIntPoint NewGridPos)
{
	GridPos = NewGridPos;



	FVector NewWorldLocation = FVector(GridPos.X * 100.0f, GridPos.Y * 100.0f, GetActorLocation().Z);

	// 3. アクターの実際の3D位置を更新する
	SetActorLocation(NewWorldLocation);
}

//移動できる範囲を生成する
void AUnit::SpawnMovePatternObject()
{

	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 3. ワールドにアクターをスポーンさせる
	AActor* SpawnedObj = GetWorld()->SpawnActor<AActor>(MovePatternObjClass, FVector(GridPos.X * 100, GridPos.Y * 100, 46), SpawnRotation);

	if (SpawnedObj)
	{
		UE_LOG(LogTemp, Warning, TEXT("移動範囲オブジェクトを生成しました！"));
	}
}

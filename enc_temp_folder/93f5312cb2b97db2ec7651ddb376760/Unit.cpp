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


//現在の位置(InTargetGrid)から攻撃できるマスを、攻撃のパターンごとに変えて全て返す配列
TArray<FIntPoint> AUnit::CalculateAttackRange(
	int32 InTargetGridX,
	int32 InTargetGridY,
	int32 InPlayerSide,
	EAtackPatterns InAtackPattern,
	int32 InAtackRange
)
{
	TArray<FIntPoint> Result;
	const int32 ForwardSign = (InPlayerSide == 0) ? 1 : -1;

	for (int32 d = 1; d <= InAtackRange; ++d)
	{
		switch (InAtackPattern)
		{
		case EAtackPatterns::Forward:
		{
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY + d * ForwardSign));
			break;
		}
		case EAtackPatterns::Cross:
		{
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY + d * ForwardSign));
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY - d * ForwardSign));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY));
			break;
		}
		case EAtackPatterns::Diagonal:
		{
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY - d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY - d));
			break;
		}
		case EAtackPatterns::All:
		{
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY));
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY - d));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY - d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY - d));
			break;
		}
		}
	}
	return Result;
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
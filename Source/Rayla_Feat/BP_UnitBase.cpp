#include "BP_UnitBase.h"

// Sets default values
ABP_UnitBase::ABP_UnitBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABP_UnitBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABP_UnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FIntPoint> ABP_UnitBase::CalculateAttackRange(
	int32 InTargetGridX,
	int32 InTargetGridY,
	int32 InPlayerSide,
	EAtackPattern InAtackPattern,
	int32 InAtackRange
)
{
	TArray<FIntPoint> Result;
	const int32 ForwardSign = (InPlayerSide == 0) ? 1 : -1;

	for (int32 d = 1; d <= InAtackRange; ++d)
	{
		switch (InAtackPattern)
		{
		case EAtackPattern::Forward:
		{
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY + d * ForwardSign));
			break;
		}
		case EAtackPattern::Cross:
		{
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY + d * ForwardSign));
			Result.Add(FIntPoint(InTargetGridX, InTargetGridY - d * ForwardSign));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY));
			break;
		}
		case EAtackPattern::Diagonal:
		{
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY + d));
			Result.Add(FIntPoint(InTargetGridX - d, InTargetGridY - d));
			Result.Add(FIntPoint(InTargetGridX + d, InTargetGridY - d));
			break;
		}
		case EAtackPattern::All:
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
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "BP_UnitBase.generated.h"

UENUM(BlueprintType)
enum class EAtackPattern : uint8
{
	Cross    UMETA(DisplayName = "Cross"),
	Forward  UMETA(DisplayName = "Forward"),
	All      UMETA(DisplayName = "All"),
	Diagonal UMETA(DisplayName = "Diagonal")
};

UCLASS()
class RAYLA_FEAT_API ABP_UnitBase : public AActor
{
	GENERATED_BODY()

public:
	ABP_UnitBase();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	TArray<FIntPoint> CalculateAttackRange(
		int32 InTargetGridX,
		int32 InTargetGridY,
		int32 InPlayerSide,
		EAtackPattern InAtackPattern,
		int32 InAtackRange
	);

	// í«â¡Ç∑ÇÈïœêî
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	int32 GridX = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	int32 GridY = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	int32 PlayerSide = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Unit")
	TArray<FIntPoint> AttackRangePriority;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
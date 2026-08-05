#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BP_UnitBase.h"
#include "MyGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FBattlePair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	AActor* UnitA = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	AActor* UnitB = nullptr;
};

UCLASS()
class RAYLA_FEAT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	TArray<FBattlePair> DetermineBattlePairs(
		AActor* P1MovedUnit,
		AActor* P2MovedUnit,
		const TArray<AActor*>& AllUnits
	);

protected:
	virtual void BeginPlay() override;

private:
	// ‡@Õ“Ë”»’è:2‘Ì‚ÌˆÚ“®æÀ•W‚ª“¯‚¶‚©
	bool CheckCollision(ABP_UnitBase* UnitA, ABP_UnitBase* UnitB) const;

	// ‡AUŒ‚”ÍˆÍ“à”»’è:Unit‚ÌAttackRangePriority‚Ì’†‚©‚çAÅ‰‚ÉŒ©‚Â‚©‚é“G‚ğ•Ô‚·
	ABP_UnitBase* FindRangeTarget(ABP_UnitBase* Unit, const TArray<AActor*>& AllUnits) const;
};
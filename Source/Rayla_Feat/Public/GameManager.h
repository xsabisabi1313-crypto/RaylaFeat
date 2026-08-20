#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Unit.h>

#include "GameManager.generated.h"


UENUM(BlueprintType)
enum class CurrentPhase : uint8
{
	EGS_Spawn   UMETA(DisplayName = "Spawn"),
	EGS_MoveReserve    UMETA(DisplayName = "MoveReserve"),
	EGS_Move    UMETA(DisplayName = "Move"),
	EGS_Battle    UMETA(DisplayName = "Battle"),
	EGS_GameOver  UMETA(DisplayName = "Game Over")
};

UCLASS()
class RAYLA_FEAT_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	virtual void Tick(float DeltaTime) override;



	// 現在のフェーズ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	CurrentPhase currentPhase;

	// 現在選択されている味方ユニット（実体）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	AUnit* SelectedUnit = nullptr;

	// 現在選択されている味方ユニットの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TSubclassOf<AActor> UnitClassToSpawn = nullptr;


	//移動予約が可能なポジションを全て返してくれる関数
	UFUNCTION(BlueprintCallable, Category = "MovePatternFunc")
	void GetAvailableMovePositions(AUnit* TargetUnit);

	// 【実体】現在計算されている移動可能ポジションのリスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovePatternFunc")
	TArray<FIntPoint> AvailableMovePositions;


	// 指定したグリッド座標が、現在移動可能なマスかどうかを判定する関数
	UFUNCTION(BlueprintCallable, Category = "MovePatternFunc")
	bool IsValidMoveDestination(FIntPoint TargetGridPos);


	// 【新規】今から移動しようと選んだ目的地の座標（仮予約用）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	FIntPoint ReserveGridPos;

	// 1. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
	TSubclassOf<class AActor> CrossMoveTile;

	//
};

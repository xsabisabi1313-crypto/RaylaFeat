#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextBlock.h"
#include "Card.h"
#include "Unit.h"
#include "BaseBuilding.h"

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

UENUM(BlueprintType)
enum class EWinner : uint8
{
	Player   UMETA(DisplayName = "Player"),
	Enemy    UMETA(DisplayName = "Enmey"),
	None    UMETA(DisplayName = "None"),
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
	virtual void Tick(float DeltaTime) override;

public:	



	// 現在のフェーズ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	CurrentPhase currentPhase;

	//勝者
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	EWinner winner = EWinner::None;

	// プレイヤーの残りコスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 PlayerCurrentCost = 10;

	// 敵の残りコスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 EnemyCurrentCost = 10; 

	// 現在選択されている味方ユニット（実体）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	AUnit* SelectedUnit = nullptr;

	// 現在選択されている味方ユニットの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TSubclassOf<AUnit> UnitClassToSpawn = nullptr;

	// 現在選択されている味方ユニット（実体）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	AUnit* SelectedEnemyUnit = nullptr;
	TSubclassOf<AUnit> EnemyUnitClassToSpawn = nullptr;


	//現在ホバーされているユニットを保持する変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	AUnit* HoveredUnit = nullptr;


	// 指定したグリッド座標が、現在移動可能なマスかどうかを判定する関数
	UFUNCTION(BlueprintCallable, Category = "MovePatternFunc")
	bool IsValidMoveDestination(FIntPoint TargetGridPos);


	// プレイヤーまたは敵が選んでいる移動予約の場所
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	FIntPoint ReserveGridPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	FIntPoint ReserveEnemyGridPos;

	// 予約した移動先へ、選択中のユニットを実際に移動させる関数
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ExecuteMove();

	// 現在マップに存在する全てのユニットを格納するリスト
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Debug")
	TArray<AUnit*> AllUnitsList;
	

	// 現在マップに存在する全ての手札カードを格納するリスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TArray<ACard*> AllCardList;

	// チーム1(自分)が持つすべてのカードを格納するリスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TArray<ACard*> PlayerCardList;

	// チーム2(敵AI)が持つすべてのカードを格納するリスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TArray<ACard*> EnemyCardList;




	// 現在マップに存在する全ての手札カードを沈ませる関数
	UFUNCTION(BlueprintCallable, Category = "Func")
	void DeselectAllCards();

	// バトル処理
	UFUNCTION(BlueprintCallable, Category = "Func")
	void ExecuteBattle(EPlayerSide AttackerSide);

	// プレイヤーが召喚できる全てのユニットの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TSubclassOf<AUnit>> PlayerOwnedUnits;


	// 敵が召喚できる全てのユニットの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TSubclassOf<AUnit>> EnemyOwnedUnits;


	//Phaseを切り替える関数
	UFUNCTION(BlueprintCallable, Category = "BP")
	void ChangePhase();


	//ゲームが終わりかを判断して、終わりならResult画面を出す関数
	UFUNCTION(BlueprintCallable, Category = "BP")
	void CheckEndAndShowResult();

	// --- リザルト画面用のWidgetクラス設計図（全体・勝利用・敗北用） ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<UUserWidget> LoseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	// プレイヤーと敵の拠点（実態）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	ABaseBuilding* PlayerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	ABaseBuilding* EnemyBase;
	


	// キャラのstatusを表示するためのWBPの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<UUserWidget> UnitTooltipWidgetClass;

	// 実際に生成して画面に出しているウィジェットの実体
	UPROPERTY()
	UUserWidget* UnitTooltipWidget = nullptr;

	// 矢印のアクタークラス（BPでアサイン用）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AActor> ArrowActorClass;

	// シーン上に実際に存在する矢印の実体
	UPROPERTY()
	AActor* ArrowActor;

	//移動予定矢印を、非表示にしたり表示させる
	UFUNCTION(BlueprintCallable, Category = "BP")
	void DisplayMoveReserveArrow(bool isActive);


};

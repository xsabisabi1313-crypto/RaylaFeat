#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"

#include "BoardManager.generated.h"

UCLASS()
class RAYLA_FEAT_API ABoardManager : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//初期のタイルを生成する関数
	UFUNCTION(BlueprintCallable, Category = "BP")
	void InitBoard();

	//タイルのアクターの設計書
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<ATile> Tile;


	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
	TArray<ATile*> AllTiles;

	// 渡された座標リストのタイルを光らせる関数
	UFUNCTION(BlueprintCallable)
	void ShowMovableRange(TArray<FIntPoint> GridPoss);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class RAYLA_FEAT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//アクターがクリックされたとき呼ぶ関数
	UFUNCTION(BlueprintCallable, Category = "Func")
	void OnMyActorClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	//このタイルのXY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FIntPoint GridXY = FIntPoint(0, 0);

	// ① タイルの見た目（3Dメッシュ）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	class UStaticMeshComponent* TileMesh;

	// 通常時のマテリアル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterialInterface* NormalMaterial;

	// ハイライト時（選択時）のマテリアル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterialInterface* HighlightMaterial;

	// ④ ハイライトの切り替え関数
	void SetHighlight(bool bIsHighlighted);


};

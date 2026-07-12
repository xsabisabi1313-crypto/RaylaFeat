// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RAYLA_FEAT_API NamikiTestClass2
{
public:
	NamikiTestClass2();
	~NamikiTestClass2();
	void TestFunction();
    // ブループリントの詳細パネルで値を変更できるようになる
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    float Health;

    // ブループリントのノードとして呼び出せるようになる
    UFUNCTION(BlueprintCallable, Category = "MyActions")
    void TakeDamage(float Amount);
};

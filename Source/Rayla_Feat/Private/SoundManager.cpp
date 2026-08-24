// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"


void ASoundManager::PlaySE(USoundBase* Sound)
{
	if (!Sound)return;
	
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
	
}


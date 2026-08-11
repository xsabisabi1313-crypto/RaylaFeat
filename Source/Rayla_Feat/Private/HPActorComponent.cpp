// Fill out your copyright notice in the Description page of Project Settings.


#include "HPActorComponent.h"

// Sets default values for this component's properties
UHPActorComponent::UHPActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHPActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHPActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHPActorComponent::ReduceHP(float DamageAmount)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("ç°ÇÃHPÇÕ: %f"), Health);
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		// Handle death logic here
		OnHealthChanged.Broadcast(Health);
		UE_LOG(LogTemp, Warning, TEXT("ç°ÇÃHPÇÕ: %f"), Health);
	}
	else
	{
		// Handle health change logic here
	}
}
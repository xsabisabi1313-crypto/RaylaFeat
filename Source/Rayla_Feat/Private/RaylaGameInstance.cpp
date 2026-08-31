// Fill out your copyright notice in the Description page of Project Settings.

#include "RaylaGameInstance.h"

void URaylaGameInstance::SetGameState(PlayerSelectedUnitTeam NewState)
{
	CurrentGameState = NewState;
}

PlayerSelectedUnitTeam URaylaGameInstance::GetGameState() const
{
	return CurrentGameState;
}

void URaylaGameInstance::SetSelectedTeam(PlayerSelectedUnitTeam NewTeam)
{
	SelectedTeam = NewTeam;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyVehicleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYVEHICLE_API AMyVehicleGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyVehicleGameMode(const FObjectInitializer& ObjectInitializer);
};

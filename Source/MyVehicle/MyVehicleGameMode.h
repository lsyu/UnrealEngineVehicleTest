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

	/** always pick new random spawn */
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart(AController* Player) override;

	/** check if player should use spawnpoint */
	virtual bool IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "MyVehicleGameMode.h"
#include "VehiclePawn.h"

AMyVehicleGameMode::AMyVehicleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AVehiclePawn::StaticClass();
}



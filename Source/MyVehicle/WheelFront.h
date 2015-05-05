#pragma once
#include "Vehicles/VehicleWheel.h"
#include "WheelFront.generated.h"

UCLASS()
class MYVEHICLE_API UWheelFront : public UVehicleWheel
{
	GENERATED_BODY()

public:
	UWheelFront(const FObjectInitializer& ObjectInitializer);
};

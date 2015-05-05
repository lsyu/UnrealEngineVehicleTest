#pragma once
#include "Vehicles/VehicleWheel.h"
#include "WheelRear.generated.h"

UCLASS()
class MYVEHICLE_API UWheelRear : public UVehicleWheel
{
	GENERATED_BODY()

public:
	UWheelRear(const FObjectInitializer& ObjectInitializer);
};

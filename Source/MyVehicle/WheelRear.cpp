// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "WheelRear.h"

UWheelRear::UWheelRear(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 55.0f;
	ShapeWidth = 40.0f;
	SteerAngle = 0.0f;

	TireType->SetFrictionScale(2.1);

	bAffectedByHandbrake = true;
}

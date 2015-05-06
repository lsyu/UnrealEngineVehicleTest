// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "WheelFront.h"

UWheelFront::UWheelFront(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 55.0f;
	ShapeWidth = 20.0f;
	SteerAngle = 45.0f;

	TireType->SetFrictionScale(4.0f);

	bAffectedByHandbrake = false;
}

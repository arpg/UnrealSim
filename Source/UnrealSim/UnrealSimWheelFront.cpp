// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UnrealSim.h"
#include "UnrealSimWheelFront.h"

UUnrealSimWheelFront::UUnrealSimWheelFront(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 18.f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 40.f;
}

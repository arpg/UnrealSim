// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UnrealSim.h"
#include "UnrealSimWheelRear.h"

UUnrealSimWheelRear::UUnrealSimWheelRear(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 18.0f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
}

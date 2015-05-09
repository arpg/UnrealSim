// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UnrealSim.h"
#include "UnrealSimGameMode.h"
#include "UnrealSimPawn.h"
#include "UnrealSimHud.h"

AUnrealSimGameMode::AUnrealSimGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AUnrealSimPawn::StaticClass();
	HUDClass = AUnrealSimHud::StaticClass();
}

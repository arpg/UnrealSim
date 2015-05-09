// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "UnrealSimHud.generated.h"


UCLASS(config = Game)
class AUnrealSimHud : public AHUD
{
	GENERATED_BODY()

public:
	AUnrealSimHud(const FObjectInitializer& ObjectInitializer);

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};

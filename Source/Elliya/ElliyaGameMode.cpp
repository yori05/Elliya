// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ElliyaGameMode.h"
#include "ElliyaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElliyaGameMode::AElliyaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ElliyaCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

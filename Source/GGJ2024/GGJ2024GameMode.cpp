// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJ2024GameMode.h"
#include "GGJ2024Character.h"
#include "UObject/ConstructorHelpers.h"

AGGJ2024GameMode::AGGJ2024GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

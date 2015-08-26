// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "OctreeTestStation.h"
#include "OctreeTestStationGameMode.h"
#include "OctreeTestStationCharacter.h"

AOctreeTestStationGameMode::AOctreeTestStationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

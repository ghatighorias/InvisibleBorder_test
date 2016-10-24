// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "InvisibleBorder_test.h"
#include "InvisibleBorder_testGameMode.h"
#include "InvisibleBorder_testCharacter.h"

AInvisibleBorder_testGameMode::AInvisibleBorder_testGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "MymGameMode.h"
#include "MymCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMymGameMode::AMymGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

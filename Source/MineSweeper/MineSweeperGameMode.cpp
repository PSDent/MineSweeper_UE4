// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MineSweeperGameMode.h"
#include "MineSweeperPlayerController.h"
#include "MineSweeperPawn.h"

AMineSweeperGameMode::AMineSweeperGameMode()
{
	// no pawn by default
	DefaultPawnClass = AMineSweeperPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMineSweeperPlayerController::StaticClass();
}

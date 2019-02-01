// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MineSweeperPlayerController.h"

AMineSweeperPlayerController::AMineSweeperPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
	DefaultMouseCursor = EMouseCursor::Default;
}

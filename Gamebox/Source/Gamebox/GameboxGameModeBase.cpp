// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameboxGameModeBase.h"
#include "SandboxPawn.h"
#include "SandboxPlayerController.h"

AGameboxGameModeBase::AGameboxGameModeBase()
{
	DefaultPawnClass = ASandboxPawn::StaticClass();
	PlayerControllerClass = ASandboxPlayerController::StaticClass();
}

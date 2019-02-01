// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MineSweeperPawn.h"
//#include "MineSweeperBlock.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
//#include "MineSweeperBlockGrid.h"
#include "UStruct1.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AMineSweeperPawn::AMineSweeperPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	isFirstClick = false;

	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);


	
}

void AMineSweeperPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UCameraComponent* OurCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
				TraceForBlock(Start, End, true);
			}
		}
		else
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);
			TraceForBlock(Start, End, false);
		}
	}
	
	CheckWin();
}

void AMineSweeperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("OnResetVR", EInputEvent::IE_Pressed, this, &AMineSweeperPawn::OnResetVR);
	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AMineSweeperPawn::RightClick);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AMineSweeperPawn::ReleaseRight);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMineSweeperPawn::LeftClick);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AMineSweeperPawn::ReleaseLeft);

}

void AMineSweeperPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AMineSweeperPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMineSweeperPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		AMineSweeperBlock* HitBlock = Cast<AMineSweeperBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}

void AMineSweeperPawn::LeftClick()
{
	leftClick = true;
	if (CurrentBlockFocus)
	{
		if (!isFirstClick)
		{
			grid->DeployMine(CurrentBlockFocus->x, CurrentBlockFocus->y);
			isFirstClick = true;
		}
		bool rs = CurrentBlockFocus->HandleClicked();
		if (rs)
		{
			((UGameOverWidget*)gameMgr->overUI)->SetResult(false);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("A LOSE"));

		}
	}
}

void AMineSweeperPawn::CheckWin()
{
	static bool check = false;
	if (grid == NULL) return;

	if (grid->bWin && !check)
	{
		((UGameOverWidget*)gameMgr->overUI)->SetResult(true);
		check = true;
	}
}

void AMineSweeperPawn::ReleaseLeft()
{
	leftClick = false;
}

void AMineSweeperPawn::RightClick()
{
	rightClick = true;

	if (CurrentBlockFocus == NULL) return;

	if (leftClick)
	{
		if (CurrentBlockFocus && CurrentBlockFocus->bIsActive)
		{
			bool rs = grid->SearchAround(CurrentBlockFocus->x, CurrentBlockFocus->y);
			if (!rs)
			{
				
				((UGameOverWidget*)gameMgr->overUI)->SetResult(false);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("LOSE"));
				//overUI->SetStatus
				// 게임 오버
			}

		}
	}
	else if (!CurrentBlockFocus->bIsActive)
	{
		if (flagAmount == 0 && CurrentBlockFocus->bMarked)
		{
			CurrentBlockFocus->MarkClicked();
			flagAmount += 1;
		}
		else if(flagAmount > 0)
		{
			bool rs = CurrentBlockFocus->MarkClicked();
			if (rs)
			{
				flagAmount -= 1;
				grid->CheckWin();
			}
			else
				flagAmount += 1;
		}
	}
}

void AMineSweeperPawn::ReleaseRight()
{
	rightClick = false;
}
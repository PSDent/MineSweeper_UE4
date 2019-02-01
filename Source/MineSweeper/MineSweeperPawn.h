// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MineSweeperBlockGrid.h"
#include "GameManager.h"
#include "MineSweeperPawn.generated.h"

UCLASS(config=Game)
class AMineSweeperPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void LeftClick();
	void ReleaseLeft();
	void RightClick();
	void ReleaseRight();

public:
	UPROPERTY(BlueprintReadWrite)
	class AMineSweeperBlockGrid *grid;

	UPROPERTY(BlueprintReadWrite)
		int flagAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGameManager *gameMgr;

	void CheckWin();

private:
	bool isFirstClick;
	bool leftClick;
	bool rightClick;
	APlayerController *controller;


protected:
	void OnResetVR();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AMineSweeperBlock* CurrentBlockFocus;
};

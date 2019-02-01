// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UStruct1.generated.h"
/**
 * 
 */
USTRUCT()
struct FPoint
{
	GENERATED_BODY()

		UPROPERTY()
		int posX;

	UPROPERTY()
		int posY;

	FPoint()
	{
		posX = 0;
		posY = 0;
	}

	FPoint(int x, int y)
	{
		posX = x;
		posY = y;
	}
};
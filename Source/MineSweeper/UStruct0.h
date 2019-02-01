// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MineSweeperBlock.h"
#include "UStruct0.generated.h"

/**
 * 
 */
USTRUCT()
struct FBlockMap
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AMineSweeperBlock*> map;
};
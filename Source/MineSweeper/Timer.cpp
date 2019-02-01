// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	time = 0;
	save = 0;
}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	save += DeltaTime;
	time = save;

	timeStr = FString::Printf(TEXT("%d : %d"), time / 60, time - (time / 60) * 60);

}


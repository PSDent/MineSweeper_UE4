// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "MineSweeperBlockGrid.h"
#include "Engine.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();

	//TSubclassOf<AMineSweeperBlockGrid> classToFind;
	//TArray<AActor*> foundActors;

	//UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AMineSweeperBlockGrid::StaticClass(), foundActors);
	//AMineSweeperBlockGrid* grid = (AMineSweeperBlockGrid*)foundActors[0];
	//grid->gameMode;
	//FString str = FString::FromInt(grid->gameMode);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, str);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HELLO"));

}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraManager::CameraSetup(int size, FVector2D pos)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Camera Set Up"));
	FString str = FString::FromInt(size);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, str);

	camera->SetActorLocation(FVector(pos.X, pos.Y, camera->GetActorLocation().Z));

	//AMineSweeperBlockGrid::MODE_SIZE[mode];
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{
	//ConstructorHelpers::FObjectFinder<UClass> widget(TEXT("/Game/BluePrint/GameOverWIdget.GameOverWidget"));

	//UClass *myWidget = (UClass*)widget.Object;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	overUI = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), a);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


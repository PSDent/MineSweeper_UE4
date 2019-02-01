// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MineSweeperBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameOverWidget.h"
#include "Engine.h"
#include "GameManager.generated.h"

UCLASS()
class MINESWEEPER_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:
	AMineSweeperBlockGrid *grid;

public:	
	// Sets default values for this actor's properties
	AGameManager();

	UUserWidget *overUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> a;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

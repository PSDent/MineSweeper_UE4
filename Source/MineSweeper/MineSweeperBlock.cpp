// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MineSweeperBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine.h"
#include "MineSweeperBlockGrid.h"
#include "Materials/MaterialInstance.h"

#define LOCTEXT_NAMESPACE "Block"

AMineSweeperBlock::AMineSweeperBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> FlagMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, FlagMaterial(TEXT("/Game/Puzzle/Meshes/FlagMaterial.FlagMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(0.15f,0.15f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	//BlockMesh->OnClicked.AddDynamic(this, &AMineSweeperBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMineSweeperBlock::OnFingerPressedBlock);
	
	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	FlagMaterial = ConstructorStatics.FlagMaterial.Get();

	mineNumText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NearMineNum"));
	mineNumText->SetRelativeLocation(FVector(-20.0f, -15.0f, 80.0f));
	mineNumText->SetRelativeRotation(FRotator(90.0f, 0, 180.0f));
	mineNumText->SetText("1");
	mineNumText->SetWorldSize(40.0f);
	mineNumText->SetTextRenderColor(FColor::Red);
	mineNumText->SetupAttachment(RootComponent);
	mineNumText->SetHiddenInGame(true);

	bMarked = false; 
	bClicked = false;
	visible = false;
	isMine = false;
}

void AMineSweeperBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMineSweeperBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

bool AMineSweeperBlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive)
	{
		bClicked = true;
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);
		mineNumText->SetHiddenInGame(false);

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->CheckWin();
			OwningGrid->AddScore();
		}

	}

	return isMine;
}

bool AMineSweeperBlock::MarkClicked()
{
	if (bMarked)
	{
		bMarked = false;
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		if (OwningGrid)
			OwningGrid->CheckWin();
		bMarked = true;
		BlockMesh->SetMaterial(0, FlagMaterial);
	}

	return bMarked;
}

void AMineSweeperBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive || bMarked)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

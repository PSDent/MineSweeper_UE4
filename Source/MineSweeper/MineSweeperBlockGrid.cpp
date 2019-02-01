// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MineSweeperBlockGrid.h"
#include "MineSweeperBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "CameraManager.h"
#include "Engine.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMineSweeperBlockGrid::AMineSweeperBlockGrid()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"));

	usedBlockMaterial = (UMaterialInstance*)Material.Object;
	if(usedBlockMaterial == NULL)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("AS"));


	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//// Create static mesh component
	//ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	//ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	//ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	//ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	//ScoreText->SetupAttachment(DummyRoot);

	bWin = false;
	bStart = false;
	bClickedFirstBlock = false;
	BlockSpacing = 40.f;
}


void AMineSweeperBlockGrid::BeginPlay()
{
	Super::BeginPlay();

}

void AMineSweeperBlockGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CheckFirstClick();

}

bool AMineSweeperBlockGrid::SearchAround(int cx, int cy)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("COMPE"));

	for (int m = -1; m < 2; ++m)
	{
		for (int n = -1; n < 2; ++n)
		{
			int y = cy + m, x = cx + n;
			// 범위 확인
			if ((m == 0 && n == 0) || y >= Size || y < 0 || x >= Size || x < 0) continue;
			if (map[y].map[x]->isMine && !map[y].map[x]->bMarked)
			{
				// 게임 오버 로직 삽입
				return false;
			}

			Search(x, y);
			CheckWin();
		}
	}

	return true;
}

void AMineSweeperBlockGrid::CheckWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("COMPE"));

	int cnt = 0;

	AMineSweeperBlock *block;
	for (int i = 0; i < Size; ++i)
	{
		for (int j = 0; j < Size; ++j)
		{
			block = map[i].map[j];
			if (block->bIsActive)
				++cnt;
			else if (block->isMine && block->bMarked)
				++cnt;
		}
	}

	if (cnt == Size * Size)
		bWin = true;
}

void AMineSweeperBlockGrid::AddScore()
{
	// Increment score
	Score++;

	//// Update text
	//ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AMineSweeperBlockGrid::CreateGrid()
{
	TArray<AActor*> foundActors;

	// Number of blocks

	// 블록을 담을 배열 map을 동적할당.
	Size = MODE_SIZE[gameMode];
	map = new FBlockMap[Size];

	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMineSweeperBlock* NewBlock = GetWorld()->SpawnActor<AMineSweeperBlock>(BlockLocation, FRotator(0, 0, 0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->x = BlockIndex / Size;
			NewBlock->y = BlockIndex % Size;
			map[BlockIndex % Size].map.Add(NewBlock);
			NewBlock->OwningGrid = this;
		}
	}

	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ACameraManager::StaticClass(), foundActors);
	ACameraManager* cameraMgr = (ACameraManager*)foundActors[0];
	int x = map[Size / 2].map[Size / 2]->GetActorLocation().X;
	int y = map[Size / 2].map[Size / 2]->GetActorLocation().Y;

	cameraMgr->CameraSetup(MODE_SIZE[gameMode], FVector2D(x, y));
}

void AMineSweeperBlockGrid::DeployMine(int clickedX, int clickedY)
{
	int totalMine = MODE_MINE[gameMode];
	int size = MODE_SIZE[gameMode];

	int limitL = clickedX - 2, limitR = clickedX + 2;
	int limitU = clickedY + 2, limitD = clickedY - 2;
	for (int i = 0; i < totalMine; ++i)
	{
		int x = clickedX, y = clickedY;
		//while (x == clickedX && y == clickedY)
		while(map[y].map[x]->isMine || (x <= limitR && x >= limitL && y <= limitU && y >= limitD))
		{
			x = FMath::RandRange(0, size - 1);
			y = FMath::RandRange(0, size - 1);
		}
		map[y].map[x]->isMine = true;
	}

	for (int i = 0; i < Size; ++i)
	{
		for (int j = 0; j < Size; ++j)
		{
			AMineSweeperBlock *block = map[i].map[j];
			// 지뢰라면 지뢰 카운팅 X
			if (block->isMine) {
				block->mineNumText->SetText("*");
				continue;
			}

			int cnt = 0;
			for (int m = -1; m < 2; ++m)
			{
				for (int n = -1; n < 2; ++n)
				{
					int y = i + m, x = j + n;
					// 범위 확인
					if ((m == 0 && n == 0) || y >= Size || y < 0 || x >= Size || x < 0) continue;
					
					if (map[y].map[x]->isMine) ++cnt;
				}
			}

			block->nearMineNum = cnt;
			if(cnt > 0)
				block->mineNumText->SetText(FString::FromInt(cnt));
			else
				block->mineNumText->SetText("");

			//block->mineNumText->SetHiddenInGame(false);
		}
	}

	Search(clickedX, clickedY);

	bStart = true;
}

void AMineSweeperBlockGrid::Search(int x, int y)
{
	if (x >= Size || x < 0 || y >= Size || y < 0 || visited[y][x] || map[y].map[x]->isMine)
		return;

	FTimerHandle h;
	GetWorld()->GetTimerManager().SetTimer(h, [=]() {
		visited[y][x] = true;
		map[y].map[x]->mineNumText->SetHiddenInGame(false);
		map[y].map[x]->GetBlockMesh()->SetMaterial(0, usedBlockMaterial);
		map[y].map[x]->bIsActive = true;

		if (map[y].map[x]->nearMineNum > 0)
			return;

		for (int i = 0; i < 4; ++i)
			Search(x + SEARCH_X[i], y + SEARCH_Y[i]);
	}, 1.0f, false, 0.02f);
}

int AMineSweeperBlockGrid::GetMineSize(int num)
{
	return MODE_MINE[num];
}


#undef LOCTEXT_NAMESPACE

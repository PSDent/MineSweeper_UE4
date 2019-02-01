// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MineSweeperBlock.h"
#include "UStruct0.h"
#include "MineSweeperBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMineSweeperBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

private:
	

public:
	// 클릭 당한 블록이 좌표를 반환하는 함수를 만들 고, 이를 Grid 객체에서 탐색을 할 것.
	// 지뢰 랜덤 배치 함수 작성

	AMineSweeperBlockGrid();

	virtual void Tick(float DeltaTime) override;

	void DeployMine(int clickedX, int clickedY);

	bool SearchAround(int cx, int cy);

	void Search(int x, int y);

	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int gameMode;

	UMaterialInstance *usedBlockMaterial;

	FBlockMap *map;

	bool bClickedFirstBlock;
	bool bStart;
	bool bWin;
	bool visited[26][26];

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	const int MODE_SIZE[3] = { 7, 15, 25 };
	int MODE_MINE[3] = { 10, 40, 120 };
	const int SEARCH_X[4] = { 0, 1, 0, -1 };
	const int SEARCH_Y[4] = { 1, 0, -1, 0 };
	/** Handle the block being clicked */
	void AddScore();
	void CheckWin();

	UFUNCTION(BlueprintCallable)
	int GetMineSize(int num);

	UFUNCTION(BlueprintCallable)
		void CreateGrid();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }
};

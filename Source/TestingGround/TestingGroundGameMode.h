// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestingGroundGameMode.generated.h"

UCLASS(minimalapi)
class ATestingGroundGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestingGroundGameMode();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Classes",meta = (DisplayName = "Tile Class"))
	TSubclassOf<class ATile> TileBlueprint;

	UFUNCTION(BlueprintCallable)
	class ATile* SpawnTile();

	UFUNCTION(BlueprintCallable,Category= "Bounds Pool")
	void PopulateBoundsVolumePool();

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	void IncreaseScore();

	// Return the player score
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	FString GetScoreToString();

	// Return the player score
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	int32 GetScore();
protected:
	virtual void BeginPlay() override;
private:

	//Next tile world transform
	FVector NextTileLocation = FVector::ZeroVector;

	class UActorPoolComponent* ActorPool;
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);

	int32 Score;
};




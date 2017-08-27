// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestingGroundGameMode.h"
#include "TestingGroundHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "Terrain/Tile.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"

ATestingGroundGameMode::ATestingGroundGameMode()
	: Super()
{
}

ATile* ATestingGroundGameMode::SpawnTile()
{
	check(TileBlueprint != nullptr && "Did you forget to specify a blueprint for the tile")
	if (TileBlueprint)
	{
		if (GetWorld())
		{
			ATile* Tile = GetWorld()->SpawnActor<ATile>(TileBlueprint);
			if (Tile)
			{
				Tile->SetActorTransform(NextTileTransform);
				NextTileTransform = Tile->GetAttachLocation();
				Tile->Lock();
			}
			return Tile;
		}
	}
	return nullptr;
}
void ATestingGroundGameMode::BeginPlay()
{
	Super::BeginPlay();
	ATile* CurrentTile = SpawnTile();
	if (CurrentTile)
	{
		CurrentTile->UnLock();
	}
	SpawnTile();
}

void ATestingGroundGameMode::AddToPool(ANavMeshBoundsVolume *NavMeshBoundsVolume)
{
	UE_LOG(LogTemp, Warning, TEXT("Found NavMesh: %s"), *NavMeshBoundsVolume->GetName())
}

void ATestingGroundGameMode::PopulateBoundsVolumePool()
{
	TActorIterator<ANavMeshBoundsVolume> NavMeshIterator(GetWorld());
	while (NavMeshIterator)
	{
		AddToPool(*NavMeshIterator);
		++NavMeshIterator;
	}
}
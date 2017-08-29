// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnParams.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUND_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Min of the box for spawning props
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Setup")
	class UArrowComponent* MinSpawnPoint;
	// Max of the box for spawning props
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	class UArrowComponent* MaxSpawnPoint;
	// Where the tile gonna be attach
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	class UArrowComponent* AttachLocation;

	//Places Actors in the world
	UFUNCTION(BlueprintCallable,Category="Spawning")
	void PlaceActors(TSubclassOf<AActor> ToSpawn,const FSpawnParams SpawnParams);
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	//Places IA in the world
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, const FSpawnParams SpawnParams);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get the next title attachement point
	UFUNCTION(BlueprintCallable)
	FTransform GetAttachLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnLock();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Lock();

	void SetActorPool(class UActorPoolComponent*);

	void PositionNavMeshBoundsVolume();

	TArray<FTransform> GenerateSpawnPosition(const FSpawnParams&);

private:
	// Cast a sphere and return true if the sphere hit a object
	bool CanSpawnAtLocation(FVector Location, float Radius);

	// Randomly Place an actor in the world
	void PlaceActor(TSubclassOf<AActor> ToSpawn, const struct FTransform&);

	// Randomly Place an actor in the world and init is AI controller
	void PlacePawn(TSubclassOf<APawn> &ToSpawn, const FTransform & SpawnPosition);

	// Get a empty location
	bool FindEmptyLocation(float Radius,FVector& OutLocation);

	// Reference to the actor pool singleton
	class UActorPoolComponent* ActorPool;

	// reference to the NavMeshBoundsVolume get from the pool. This thing need to be returned on end play
	class AActor* NavMeshBoundsVolume;
};

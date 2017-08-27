// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UFUNCTION(BlueprintCallable,Category="Setup")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinScale = 1.0f,float MaxScale= 1.0f,float CollisionRadius = 500.0f);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	// Cast a sphere and return true if the sphere hit a object
	bool CanSpawnAtLocation(FVector Location, float Radius);

	//Place an actor in the world
	void PlaceActor(TSubclassOf<AActor> ToSpawn,FVector Location,float YawRotation, float Scale);

	// Get a empty location
	bool FindEmptyLocation(float Radius,FVector& OutLocation);
};

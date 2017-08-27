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
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	// Cast a sphere and return true if the sphere hit a object
	bool CastSphere(FVector Location, float Radius);
};

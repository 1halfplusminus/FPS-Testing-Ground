// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SubclassOf.h"
#include "DrawDebugHelpers.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(FName("Shared Root")));

	MinSpawnPoint = CreateDefaultSubobject<UArrowComponent>(FName("FBox Min"));
	MinSpawnPoint->SetupAttachment(RootComponent);
	MaxSpawnPoint = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("FBox Max"));
	MaxSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn,int32 MinSpawn,int32 MaxSpawn)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	FBox Bounds = FBox(MinSpawnPoint->RelativeLocation, MaxSpawnPoint->RelativeLocation);
	for (int32 ActorCount = 0; ActorCount <= NumberToSpawn; ActorCount++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		if (Spawned)
		{
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			Spawned->SetActorRelativeLocation(SpawnPoint);
			CastSphere(SpawnPoint, 300);
		}
	}
}

bool  ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, 
		Location, 
		Location, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = (HasHit)?FColor::Red: FColor::Green;
	DrawDebugSphere(
		GetWorld(),
		Location,
		Radius,
		34,
		ResultColor,
		true,
		100
	);
	return HasHit;
}
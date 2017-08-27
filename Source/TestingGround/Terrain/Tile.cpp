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
	PrimaryActorTick.bCanEverTick = false;

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

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn,int32 MinSpawn,int32 MaxSpawn, float MinScale, float MaxScale,float CollisionRadius)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 ActorCount = 0; ActorCount <= NumberToSpawn; ActorCount++)
	{
		FVector Location;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(CollisionRadius * RandomScale, Location))
		{
			float RandomRotation = FMath::RandRange(-100.0f,100.0f);
			PlaceActor(ToSpawn, Location, RandomRotation,RandomScale);
		}
	}
}

bool  ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalPosition = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GlobalPosition,
		GlobalPosition,
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = (HasHit)?FColor::Red: FColor::Green;
	//DrawDebugSphere(
	//	GetWorld(),
	//	GlobalPosition,
	//	Radius,
	//	12,
	//	ResultColor,
	//	true,
	//	100
	//);
	return !HasHit;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector Location, float YawRotation,float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRelativeLocation(Location);
		Spawned->SetActorRotation(FRotator(0,YawRotation,0));
		Spawned->SetActorScale3D(FVector(Scale));
	}
}

bool ATile::FindEmptyLocation(float Radius, FVector &OutLocation)
{
	FBox Bounds = FBox(MinSpawnPoint->RelativeLocation, MaxSpawnPoint->RelativeLocation);
	const int MAX_ATTEMPS = 20;
	for (size_t NbOfAttemp = 0; NbOfAttemp < MAX_ATTEMPS; NbOfAttemp++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(SpawnPoint, Radius))
		{
			OutLocation = SpawnPoint;
			return true;
		}
	}

	return false;
}

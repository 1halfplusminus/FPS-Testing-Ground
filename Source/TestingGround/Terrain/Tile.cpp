// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SubclassOf.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolComponent.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorPool = nullptr;
	NavMeshBoundsVolume = nullptr;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(FName("Shared Root")));

	MinSpawnPoint = CreateDefaultSubobject<UArrowComponent>(FName("FBox Min"));
	MinSpawnPoint->SetupAttachment(RootComponent);

	MaxSpawnPoint = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("FBox Max"));
	MaxSpawnPoint->SetupAttachment(RootComponent);

	AttachLocation = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("Attach Location"));
	AttachLocation->SetupAttachment(RootComponent);
	AttachLocation->SetRelativeLocation(FVector(3675.0f,0.0f,0.0f));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ActorPool && NavMeshBoundsVolume) {
		ActorPool->Return(NavMeshBoundsVolume);
	}
	UE_LOG(LogTemp,Warning,TEXT("[%s] EndPlay"),*GetName());
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PositionNavMeshBoundsVolume()
{
	check(ActorPool != nullptr)
	if (ActorPool)
	{
		NavMeshBoundsVolume = ActorPool->Checkout();
		if (NavMeshBoundsVolume)
		{
			NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
			GetWorld()->GetNavigationSystem()->Build();
		}
	}
}

void ATile::SetActorPool(UActorPoolComponent* ActorPoolToSet)
{
	check(ActorPoolToSet != nullptr)
	ActorPool = ActorPoolToSet;
}

FTransform ATile::GetAttachLocation() const
{
	return AttachLocation->GetComponentTransform();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn,const FSpawnParams SpawnParams)
{
	RandomlyPlaceActors(ToSpawn, SpawnParams);
}
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn,const FSpawnParams SpawnParams)
{
	RandomlyPlaceActors(ToSpawn, SpawnParams);
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

template<>
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn,const FTransform& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorTransform(SpawnPosition);
	}
}
template<>
void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FTransform& SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorTransform(SpawnPosition);
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}

bool ATile::FindEmptyLocation(float Radius, FVector &OutLocation)
{
	FBox Bounds = FBox(MinSpawnPoint->RelativeLocation, MaxSpawnPoint->RelativeLocation);
	//TODO PUT CONST IN HEADER FILE ?
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

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FSpawnParams SpawnParams)
{
	int32 NumberToSpawn = FMath::RandRange(SpawnParams.MinSpawn, SpawnParams.MaxSpawn);
	for (int32 ActorCount = 0; ActorCount < NumberToSpawn; ActorCount++)
	{
		FVector Location;
		if (FindEmptyLocation(SpawnParams.CollisionRadius * FMath::Max3(SpawnParams.MaxTransform.Scale.X, SpawnParams.MaxTransform.Scale.Y, SpawnParams.MaxTransform.Scale.Z), Location))
		{
			FVector Scale;
			Scale.X = FMath::RandRange(SpawnParams.MinTransform.Scale.X, SpawnParams.MaxTransform.Scale.X);
			Scale.Y = FMath::RandRange(SpawnParams.MinTransform.Scale.Y, SpawnParams.MaxTransform.Scale.Y);
			Scale.Z = FMath::RandRange(SpawnParams.MinTransform.Scale.Z, SpawnParams.MaxTransform.Scale.Z);
			FRotator Rot;
			Rot.Roll = FMath::RandRange(SpawnParams.MinTransform.Rotation.Roll, SpawnParams.MaxTransform.Rotation.Roll);
			Rot.Pitch = FMath::RandRange(SpawnParams.MinTransform.Rotation.Pitch, SpawnParams.MaxTransform.Rotation.Pitch);
			Rot.Yaw = FMath::RandRange(SpawnParams.MinTransform.Rotation.Yaw, SpawnParams.MaxTransform.Rotation.Yaw);
			FTransform SpawnPosition(Rot, Location, Scale);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}
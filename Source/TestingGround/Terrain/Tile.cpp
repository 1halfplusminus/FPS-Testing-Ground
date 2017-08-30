// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "TestingGroundGameMode.h"
#include "ActorPoolComponent.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorPool = nullptr;
	NavMeshBoundsVolume = nullptr;
	IsTileConquered = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(BoxComponent);

	AttachLocation = CreateDefaultSubobject<UArrowComponent>(FName("Attach Location"));
	AttachLocation->SetupAttachment(BoxComponent);
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
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::TileConquered()
{
	if (IsTileConquered == false)
	{
		if (GetWorld())
		{
			ATestingGroundGameMode* GameMode = (ATestingGroundGameMode*)GetWorld()->GetAuthGameMode();
			if (ensure(GameMode != nullptr))
			{
				GameMode->IncreaseScore();
			}
		}
		IsTileConquered = true;
	}
}

void ATile::PositionNavMeshBoundsVolume()
{
	check(ActorPool != nullptr)
	if (ActorPool)
	{
		NavMeshBoundsVolume = ActorPool->Checkout();
		if (NavMeshBoundsVolume)
		{
			UE_LOG(LogTemp,Warning,TEXT("Nav mesh bound volume %s"),*GetActorLocation().ToCompactString())
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

FVector ATile::GetAttachLocation() const
{
	return AttachLocation->GetComponentLocation();
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
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Location,
		Location,
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = (HasHit)?FColor::Red: FColor::Green;
	//DrawDebugSphere(
	//	GetWorld(),
	//	Location,
	//	Radius,
	//	24,
	//	ResultColor,
	//	true,
	//	100
	//);
	return !HasHit;
}

template<>
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn,const FTransform& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPosition.GetLocation(), FRotator::ZeroRotator);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorTransform(SpawnPosition);
	}
}
template<>
void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FTransform& SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn,SpawnPosition.GetLocation(),FRotator::ZeroRotator);
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
	FBox Bounds = RootComponent->Bounds.GetBox();
	//TODO PUT CONST IN HEADER FILE ?
	const int MAX_ATTEMPS = 10;
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
// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
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

void ATile::PlaceActors()
{
	FVector RandomPoint = FMath::RandPointInBox(FBox(MinSpawnPoint->GetComponentLocation(), MaxSpawnPoint->GetComponentLocation()));
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(),RandomPoint,100,FColor(255,0,255),99999999.0f);
}
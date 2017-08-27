// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values for this component's properties
UGrassComponent::UGrassComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FoliageGenerator = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(FName("FoliageGenerator"));
	FoliageGenerator->SetupAttachment(GetAttachmentRoot());
	FoliageGenerator->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


// Called when the game starts
void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();
	FoliageGenerator->SetStaticMesh(Mesh);
	GenerateFoliage();
}


// Called every frame
void UGrassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrassComponent::GenerateFoliage()
{
	FBox Box = CalcBounds(FTransform()).GetBox();
	for (int32 Count = 0; Count <= SpawnCount; Count++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Box);
		FoliageGenerator->AddInstance(FTransform(SpawnPoint));
	}
}
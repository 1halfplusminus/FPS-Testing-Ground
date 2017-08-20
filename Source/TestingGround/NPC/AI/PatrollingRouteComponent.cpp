// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC/AI/PatrollingRouteComponent.h"


// Sets default values for this component's properties
UPatrollingRouteComponent::UPatrollingRouteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPatrollingRouteComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentIndex = 0;
}


// Called every frame
void UPatrollingRouteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UPatrollingRouteComponent::CanPatrol()
{
	return PatrollingPoint.Num() > 0;
}
AActor* UPatrollingRouteComponent::Next() {

	AActor* NextWaypoint = PatrollingPoint[CurrentIndex];
	CurrentIndex = (CurrentIndex + 1) % PatrollingPoint.Num();
	return NextWaypoint;
}

int32 UPatrollingRouteComponent::GetCurrentIndex() const
{
	return CurrentIndex;
}
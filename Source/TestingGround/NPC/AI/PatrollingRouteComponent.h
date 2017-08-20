// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrollingRouteComponent.generated.h"

/**
Allow IAController to patrol circularly between defined set of waypoint
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUND_API UPatrollingRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrollingRouteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Return the next patrol points in the iteration
	AActor* Next();
	//Check if the component have a valid patrol routes
	bool CanPatrol();

	int32 GetCurrentIndex() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Setup")
	TArray<AActor*> PatrollingPoint;
	
	int32 CurrentIndex;
};

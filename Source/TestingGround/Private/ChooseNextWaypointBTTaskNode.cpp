// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypointBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"
#include "PatrollingGuard.h"

EBTNodeResult::Type UChooseNextWaypointBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* Controller = OwnerComp.GetAIOwner();

	// Check if the possed pawn is a patrolling guard
	APatrollingGuard* PossesedPawn = Cast<APatrollingGuard>(Controller->GetPawn());
	if (!PossesedPawn) { return EBTNodeResult::Aborted; }
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT A PATTROLING GUARD"))
	}
	
	// Get the waypoint of the patrollingGuard
	TArray<AActor*> Waypoints = PossesedPawn->PatrollingPoint;
	if (Waypoints.Num() != 0)
	{
		int32 CurrentWaypointIndex = Blackboard->GetValueAsInt(IndexKey.SelectedKeyName);
		AActor* Waypoint = Waypoints[CurrentWaypointIndex];
		// Find the next waypoitn index
		int32 NextWaypointIndex = (CurrentWaypointIndex+1) % Waypoints.Num();

		//Update the blackboard
		Blackboard->SetValueAsObject(WaypointKey.SelectedKeyName, Waypoint);
		Blackboard->SetValueAsInt(IndexKey.SelectedKeyName, NextWaypointIndex); // The current index is now the previous index + 1

		UE_LOG(LogTemp, Warning, TEXT("HERE"))
	}

	return EBTNodeResult::Succeeded;
}

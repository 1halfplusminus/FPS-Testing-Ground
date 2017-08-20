// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypointBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"
#include "PatrollingRouteComponent.h"

EBTNodeResult::Type UChooseNextWaypointBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* Controller = OwnerComp.GetAIOwner();

	// Check if the possed pawn is a patrolling guard
	AActor* PossesedPawn = Controller->GetPawn();
	if (!PossesedPawn) { return EBTNodeResult::Aborted; }
	UPatrollingRouteComponent* PatrollingComponent = PossesedPawn->FindComponentByClass<UPatrollingRouteComponent>();
	if(!PatrollingComponent) { return EBTNodeResult::Aborted;  }
	// Get the waypoint of the patrolling component
	if (PatrollingComponent->CanPatrol())
	{
		AActor* Waypoint = PatrollingComponent->Next();
		Blackboard->SetValueAsObject(WaypointKey.SelectedKeyName, Waypoint);
		Blackboard->SetValueAsInt(IndexKey.SelectedKeyName, PatrollingComponent->GetCurrentIndex());
	}
	return EBTNodeResult::Succeeded;
}

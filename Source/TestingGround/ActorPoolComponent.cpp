// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPoolComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UActorPoolComponent::UActorPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}
AActor * UActorPoolComponent::Checkout()
{	
	if (ensure(Pool.Num()  > 0 ))
	{
		AActor* Actor = Pool.Pop();
		return Actor;
	}
	UE_LOG(LogTemp, Error, TEXT("Not enough actor in the pool"))
	return nullptr;
}

void UActorPoolComponent::Return(AActor* ToReturn)
{
	check(ToReturn != nullptr)
	if (ToReturn)
	{
		Pool.Add(ToReturn);
	}
}
void UActorPoolComponent::Add(AActor* ToAdd)
{
	check(ToAdd != nullptr)
	if (ToAdd)
	{
		Pool.Add(ToAdd);
	}
}
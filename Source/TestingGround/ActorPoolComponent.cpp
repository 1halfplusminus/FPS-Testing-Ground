// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPoolComponent.h"


// Sets default values for this component's properties
UActorPoolComponent::UActorPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}
AActor * UActorPoolComponent::Checkout()
{
	return nullptr;
}

void UActorPoolComponent::Return(AActor * toReturn)
{

}
void UActorPoolComponent::Add(AActor * toAdd)
{

}
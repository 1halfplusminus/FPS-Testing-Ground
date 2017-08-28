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
	AActor* Outer = NewObject<AActor>((UObject*)GetTransientPackage(), FName("Test Actor"));
	return Outer;
}

void UActorPoolComponent::Return(AActor * ToReturn)
{
	check(ToReturn != nullptr)
	if (ToReturn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor return to the pool %s"), *ToReturn->GetName())
	}
}
void UActorPoolComponent::Add(AActor * ToAdd)
{
	check(ToAdd != nullptr)
	if (ToAdd)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor added to the pool %s"), *ToAdd->GetName())
	}
}
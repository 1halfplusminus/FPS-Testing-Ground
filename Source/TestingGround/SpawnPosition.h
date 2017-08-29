// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnPosition.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector(1);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnPosition.h"
#include "SpawnParams.generated.h"

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinSpawn = 0; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollisionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnPosition MinTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnPosition MaxTransform;
};
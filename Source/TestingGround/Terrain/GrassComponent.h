// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GrassComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUND_API UGrassComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrassComponent();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 SpawnCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	UStaticMesh* Mesh;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class UHierarchicalInstancedStaticMeshComponent* FoliageGenerator;
	void GenerateFoliage();
	
};

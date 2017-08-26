// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGroundCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Gun.h"
#include "SubclassOf.h"
#include "Animation/AnimInstance.h"

// Sets default values
ATestingGroundCharacter::ATestingGroundCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleSize(55.0f, 100.0f);
	
	GetMesh()->bOwnerNoSee = true;

	// Setup spring arms for Third player camera
	TPCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	TPCameraBoom->SetupAttachment(RootComponent);
	
	TPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	TPCamera->bAutoActivate = false;
	TPCamera->bUsePawnControlRotation = false;
	TPCamera->SetupAttachment(TPCameraBoom);

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->bAutoActivate = true;
	FPCamera->bUsePawnControlRotation =  true;
	FPCamera->SetupAttachment(RootComponent);

	FPArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	FPArms->SetOnlyOwnerSee(true);
	FPArms->CastShadow = false;
	FPArms->bCastDynamicShadow = false;
	FPArms->SetupAttachment(FPCamera);

	// Default value
	Health = 100;
}

// Called when the game starts or when spawned
void ATestingGroundCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GunTemplate)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunTemplate);
		if (IsPlayerControlled())
		{
			Gun->AttachToComponent(FPArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
	}
}

// Called every frame
void ATestingGroundCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATestingGroundCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this, &ATestingGroundCharacter::PullTrigger);
}

void ATestingGroundCharacter::PullTrigger()
{
	if (Gun)
	{
		Gun->OnFire();
		UAnimInstance* AnimInstance = (IsPlayerControlled()) ? FPArms->GetAnimInstance() : GetMesh()->GetAnimInstance();
		UAnimMontage*  FireAnimation = (IsPlayerControlled()) ? FPFireAnimation :TPFireAnimation;
		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			//Get the animation object for the arms mesh
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

bool ATestingGroundCharacter::IsDead()
{
	if (Health <= 0)
	{
		return true;
	}
	else 
	{
		return false;
	}
}
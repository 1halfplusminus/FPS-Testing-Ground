// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGroundCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Gun.h"
#include "SubclassOf.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Controller.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "Components/ArrowComponent.h"

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

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->CastShadow = false;
	FPMesh->bCastDynamicShadow = false;
	FPMesh->SetupAttachment(FPCamera);


	//TODO Find a sensible default
	//Death camera position
	DeathCameraSpawn = CreateDefaultSubobject<UArrowComponent>(TEXT("DeathCamera spawn point"));
	DeathCameraSpawn->SetupAttachment(RootComponent);

	// Default value
	Health = 100;
}

// Called when the game starts or when spawned
void ATestingGroundCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ATestingGroundCharacter::OnDamage);
	if (GunTemplate)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunTemplate);
		if (IsPlayerControlled())
		{
			Gun->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
	}
}


void  ATestingGroundCharacter::OnDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsDead())
	{
		Health = (Health - Damage >= 0) ? Health - Damage : 0;
		if (IsDead())
		{
			APlayerController* Controller = Cast<APlayerController>(GetController());
			// TODO assertion
			if (Controller && GetWorld())
			{
				// TODO Assertion
				ACameraActor* DeathCamera = GetWorld()->SpawnActor<ACameraActor>(DeathCameraSpawn->GetComponentLocation(), DeathCameraSpawn->GetComponentRotation());
				if (DeathCamera)
				{
					Controller->SetViewTargetWithBlend(DeathCamera, 1.0f);
				}

			}
			FPMesh->SetVisibility(false, true);
			DetachFromControllerPendingDestroy();
		}
		TakeDamage();
	}
}
void ATestingGroundCharacter::PullTrigger()
{
	if (Gun)
	{
		Gun->OnFire();
		UAnimInstance* AnimInstance = (IsPlayerControlled()) ? FPMesh->GetAnimInstance() : GetMesh()->GetAnimInstance();
		UAnimMontage*  FireAnimation = (IsPlayerControlled()) ? FPFireAnimation :TPFireAnimation;
		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			//Get the animation object for the mesh
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


// Called every frame
void ATestingGroundCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATestingGroundCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATestingGroundCharacter::PullTrigger);
}

void ATestingGroundCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(FPCamera->GetAttachmentRootActor());
		FPCamera->Activate();
	}
}
void ATestingGroundCharacter::UnPossessed()
{
	Super::UnPossessed();
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}
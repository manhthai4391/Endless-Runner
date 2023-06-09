// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include "EndlessRunnerGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	static FVector Impulse = FVector(0, 0, MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector location = GetCapsuleComponent()->GetComponentLocation();
	location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(location);
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::Death()
{
	if (!bIsDead) 
	{
		bIsDead = true;
		const FVector location = GetActorLocation();

		UWorld* world = GetWorld();

		if (world)
		{

			DisableInput(nullptr);
			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(world, DeathParticleSystem, location);
			}

			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(world, DeathSound, location);
			}

			GetMesh()->SetVisibility(false);

			world->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
		}
	}
}

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}

void ARunCharacter::OnDeath()
{
	if (RestartTimerHandle.IsValid()) 
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	RunGameMode->GameOver();
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator controlRot = GetControlRotation();
	controlRot.Roll = 0.f;
	controlRot.Pitch = 0.f;

	AddMovementInput(controlRot.Vector());
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Up", IE_Released, this, &ARunCharacter::StopJumping);

	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &ARunCharacter::MoveDown);
}


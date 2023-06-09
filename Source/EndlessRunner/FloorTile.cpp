// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RunCharacter.h"
#include "EndlessRunnerGameModeBase.h"
#include "Obstacle.h"
#include "CoinItem.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FLoorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

const FTransform AFloorTile::GetAttachTransform() const
{
	return AttachPoint->GetComponentTransform();
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

	if (RunCharacter) 
	{
		RunGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 2.f, false);
	}
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinItemClass))
	{
		int32 NumBigObstacles = 0;
		SpawnLaneItem(CenterLane, NumBigObstacles);
		SpawnLaneItem(LeftLane, NumBigObstacles);
		SpawnLaneItem(RightLane, NumBigObstacles);
	}
}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane, int32& NumBigObstacles)
{
	const float randomValue = FMath::FRandRange(0.f, 1.f);

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& spawnLocation = Lane->GetComponentTransform();

	if (randomValue < SpawnPercent1) 
	{
		if (NumBigObstacles <= 2)
		{
			AObstacle* bigObstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass, spawnLocation, spawnParameters);
			if (bigObstacle)
			{
				NumBigObstacles += 1;
			}
		}
		else
		{
			AObstacle* smallObstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, spawnLocation, spawnParameters);
		}
	}
	else if (randomValue < SpawnPercent2)
	{
		AObstacle* smallObstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, spawnLocation, spawnParameters);
	}
	else if (randomValue < SpawnPercent3)
	{
		ACoinItem* coin = GetWorld()->SpawnActor<ACoinItem>(CoinItemClass, spawnLocation, spawnParameters);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	this->Destroy();
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);
}


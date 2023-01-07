#include "BodyManager.h"


const FVector2D WORLD_SIZE = FVector2D(8000, 4520);
const float GRAVITY_CONSTANT = 10000.0f;
const float MINIMUM_AFFECTING_DISTANCE = 10.0f;
const float MAX_TICK = 0.0167;


ABodyManager::ABodyManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.TickGroup = TG_DuringPhysics;
	
	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));
	SetRootComponent(InstancedMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assets/CircleMeshSimple'"));
	InstancedMesh->SetStaticMesh(MeshAsset.Object);
}


void ABodyManager::InitMasses()
{
	for (int index = 0; index < MassCount; index++)
	{
		FVector2D random_position(
			FMath::FRandRange(-WORLD_SIZE.X / 2.0f, WORLD_SIZE.X / 2.0f),
			FMath::FRandRange(-WORLD_SIZE.Y / 2.0f, WORLD_SIZE.Y / 2.0f));
		FVector2D random_velocity(
			FMath::FRandRange(-400.0f, 400.0f),
			FMath::FRandRange(-400.0f, 400.0f));
		float mass = FMath::FRandRange(10.0, 100.0);
		Masses.Push({random_position, random_velocity, mass, index});

		float mesh_scale = mass * 0.005;
		FTransform transform(
			FRotator(),
			FVector(random_position.Y, random_position.X, 0.0f),
			FVector(mesh_scale, mesh_scale, 1.0));
		InstancedMesh->AddInstance(transform);
	}
}



void ABodyManager::BeginPlay()
{
	Super::BeginPlay();
	InitMasses();

}

void ABodyManager::Tick(float DeltaTime)
{
	// if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 40.0f, FColor::Green, TEXT("Tick!"));

	if (DeltaTime > MAX_TICK) {
         DeltaTime = MAX_TICK;
	}

	Super::Tick(DeltaTime);

    FVector2D half_world = WORLD_SIZE / 2.0;

    ParallelFor(Masses.Num(), [&] (int i) {
        FVector2D force(0.0, 0.0);
        for (FMassEntity& affecting_mass: Masses) {
            if (affecting_mass.id == Masses[i].id) continue; // exclude self
            float distance = FVector2D::Distance(Masses[i].Position, affecting_mass.Position);
            distance = FMath::Clamp(distance, MINIMUM_AFFECTING_DISTANCE, half_world.X); // avoids division by zero
            force += Masses[i].Mass * affecting_mass.Mass * GRAVITY_CONSTANT / distance / distance * (affecting_mass.Position - Masses[i].Position) / distance;
        }
        Masses[i].Velocity += force * DeltaTime / Masses[i].Mass;
    });


	for (FMassEntity& mass: Masses)
	{
		mass.Position += mass.Velocity * DeltaTime;
		mass.Position = FVector2D(
            FMath::Wrap(mass.Position.X, -half_world.X, half_world.X),
            FMath::Wrap(mass.Position.Y, -half_world.Y, half_world.Y)
        );
		float mesh_scale = mass.Mass * 0.005;
		FTransform transform(
			FRotator(),
			FVector(mass.Position.Y, mass.Position.X, 0.0f),
			FVector(mesh_scale, mesh_scale, 1.0)
		);
		InstancedMesh->UpdateInstanceTransform(mass.id, transform);
	}
	InstancedMesh->MarkRenderStateDirty();
}


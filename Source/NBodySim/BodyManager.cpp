#include "BodyManager.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"


// constexpr FVector2D WORLD_SIZE = FVector2D(WorldWidth, WorldWidth/);
// const FVector2D WORLD_SIZE = FVector2D(8000, 4520);

const float MINIMUM_AFFECTING_DISTANCE = 5.0f; // to prevent division by zero and too bi forces 
const float MAX_TICK = 0.0167; // to have stable simulation steps


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
	Masses.SetNumUninitialized(MassCount);
	Transforms.SetNumUninitialized(MassCount);
	for (int index = 0; index < MassCount; index++)
	{
		FVector2D random_position(
			FMath::FRandRange(-WorldWidth / 2.0f, WorldWidth / 2.0f),
			FMath::FRandRange(-WorldHeight / 2.0f, WorldHeight / 2.0f));
		FVector2D random_velocity(
			FMath::FRandRange(-MaxInitialVelocity, MaxInitialVelocity),
			FMath::FRandRange(-MaxInitialVelocity, MaxInitialVelocity));
		float mass = FMath::FRandRange(MinMass, MaxMass);

		float mesh_scale = mass * BodyDisplayScale;
		FTransform transform (
			FRotator(0.0f),
			PositionFromPlanar(random_position),
			FVector(mesh_scale, mesh_scale, 1.0f)
		);
		Transforms[index] = transform;
		int mesh_index = InstancedMesh->AddInstance(transform);
		Masses[index] = FMassEntity{random_position, random_velocity, mass, mesh_index};
	}
}


// void ABodyManager::InitSimulationPlane() {
// 	if (UWorld* World = GetWorld()) {
// 		APlayerCameraManager* CameraManager = World->GetFirstPlayerController()->PlayerCameraManager;
// 		CameraManager->SetOrthographic = true;
// 		CameraManager->SetOrthoWidth(WorldWidth);
// 		WorldWidth = CameraManager->GetOrthoWidth();
// 		WorldHeight = WorldWidth / CameraManager->DefaultAspectRatio;
// 	}
// }


void ABodyManager::BeginPlay()
{
// 	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 40.0f, FColor::Green, TEXT("Tick!"));
	Super::BeginPlay();
//	InitSimulationPlane();
	InitMasses();
}


void ABodyManager::Tick(float DeltaTime)
{
	if (DeltaTime > MAX_TICK) {
         DeltaTime = MAX_TICK;
	}
	Super::Tick(DeltaTime);

    FVector2D half_world(WorldWidth * 0.5f, WorldHeight * 0.5f);

    ParallelFor(Masses.Num(), [&] (int i) {
        FVector2D force(0.0f, 0.0f);
        for (FMassEntity& affecting_mass: Masses) {
            if (affecting_mass.id == Masses[i].id) continue; // exclude self
            float distance = FVector2D::Distance(Masses[i].Position, affecting_mass.Position);
            distance = FMath::Clamp(distance, MINIMUM_AFFECTING_DISTANCE, half_world.X); // avoids division by zero
            force += Masses[i].Mass * affecting_mass.Mass * G / distance / distance * (affecting_mass.Position - Masses[i].Position) / distance;
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
		Transforms[mass.id].SetTranslation(PositionFromPlanar(mass.Position));
		InstancedMesh->UpdateInstanceTransform(mass.id, Transforms[mass.id]);
	}
	InstancedMesh->MarkRenderStateDirty();
}


FVector ABodyManager::PositionFromPlanar(const FVector2D& plane_coordinates) {
	return FVector(plane_coordinates.Y, plane_coordinates.X, 0.0f);
}


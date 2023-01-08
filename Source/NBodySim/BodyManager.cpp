#include "BodyManager.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"


const float MINIMUM_AFFECTING_DISTANCE = 10.0f; // to prevent division by zero and too high forces 
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


void ABodyManager::InitBodies()
{
	Bodies.SetNumUninitialized(BodyNum);
	Transforms.SetNumUninitialized(BodyNum);
	for (int index = 0; index < BodyNum; ++index)
	{
		FVector2D random_position(FMath::RandPointInCircle(PlacementRadius));
		FVector2D random_velocity(FMath::RandPointInCircle(MaxInitialVelocity));
		float mass = FMath::FRandRange(MinMass, MaxMass);
		float mesh_scale = FMath::Sqrt(mass) * BodyDisplayScale;
		FTransform transform (
			FRotator(),
			TranslationFrom2DCoordinates(random_position),
			FVector(mesh_scale, mesh_scale, 1.0f)
		);
		Transforms[index] = transform;
		Bodies[index] = FBodyEntity{random_position, random_velocity, mass, index};
	}
	InstancedMesh->AddInstances(Transforms, false);

}


void ABodyManager::BeginPlay()
{
	Super::BeginPlay();
	InitBodies();
}


void ABodyManager::BuildBHTree() {

}


void ABodyManager::GravityStep(float DeltaTime)
{
	float MaxGravityDistance = WorldWidth;
    ParallelFor(Bodies.Num(), [&] (int i) {
        FVector2D acceleration(0.0f, 0.0f);
        for (FBodyEntity& affecting_body: Bodies) {
            if (affecting_body.Index == Bodies[i].Index) continue; // exclude self
            float distance = FVector2D::Distance(Bodies[i].Position, affecting_body.Position);
            distance = FMath::Max(distance, MINIMUM_AFFECTING_DISTANCE); // avoids division by zero
            acceleration += affecting_body.Mass / distance * G / distance * (affecting_body.Position - Bodies[i].Position) / distance;
        }
        Bodies[i].Velocity += acceleration * DeltaTime ;
    });
}


void ABodyManager::UpdatePositionStep(float DeltaTime)
{
    FVector2D half_world(WorldWidth * 0.5f, WorldHeight * 0.5f);
	for (FBodyEntity& body: Bodies)
	{
		body.Position += body.Velocity * DeltaTime;
		body.Position.X = FMath::Wrap(body.Position.X, -half_world.X, half_world.X);
		body.Position.Y = FMath::Wrap(body.Position.Y, -half_world.Y, half_world.Y);
		Transforms[body.Index].SetTranslation(TranslationFrom2DCoordinates(body.Position));
	}
	InstancedMesh->BatchUpdateInstancesTransforms(0, Transforms, false, true);
}


void ABodyManager::Tick(float DeltaTime)
{
// 	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 40.0f, FColor::Green, TEXT("Tick!"));
	if (DeltaTime > MAX_TICK) {
         DeltaTime = MAX_TICK;
	}
	Super::Tick(DeltaTime);
	if (IsGravityEnabled) {
		GravityStep(DeltaTime);
	}
	UpdatePositionStep(DeltaTime);
}


FVector ABodyManager::TranslationFrom2DCoordinates(const FVector2D& XYCoordinates) {
	return FVector(XYCoordinates.Y, XYCoordinates.X, 0.0f);
}

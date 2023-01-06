#include "Mass.h"


// TODO: change to StaticMeshActor
// TODO: manipulate struct array from the GameMode
// TODO: nbody physics? parallelism?  


const float MESH_SCALE = 0.004;

AMass::AMass()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assets/CircleMesh'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MassMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetStaticMesh(MeshAsset.Object);
}

void AMass::SetMass(float NewMass)
{
	Mass = NewMass;
	float scale = Mass * MESH_SCALE;
	StaticMesh->SetRelativeScale3D(FVector(scale, scale, 1.0f));
}


void AMass::BeginPlay()
{
	Super::BeginPlay();
	UpdatePosition();
}


void AMass::Tick(float DeltaSecs)
{
	Super::Tick(DeltaSecs);
	UpdatePosition();
}


UStaticMeshComponent* AMass::GetMesh() {
	return StaticMesh;
}

FVector AMass::LocationFrom2DPosition(FVector2D planar_position) {
	return FVector(planar_position.Y, planar_position.X, 0.0f);
}


void AMass::UpdatePosition() {
	StaticMesh->SetWorldLocation(AMass::LocationFrom2DPosition(Position));
}
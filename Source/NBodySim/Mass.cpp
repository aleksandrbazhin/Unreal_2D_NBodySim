#include "Mass.h"


// TODO: change to StaticMeshActor

// TODO: load mesh to static mesh

// TODO: manipulate struct array from the GameMode

// TODO: nbody physics? parallelism?  



AMass::AMass()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assets/CircleMesh'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MassMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetStaticMesh(MeshAsset.Object);
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
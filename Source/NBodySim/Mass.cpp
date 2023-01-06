#include "Mass.h"


// TODO: change to StaticMeshActor

// TODO: load mesh to static mesh

// TODO: manipulate struct array from the GameMode

// TODO: nbody physics? parallelism?  

AMass::AMass()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MassMesh"));
	RootComponent = mesh;
	mesh->SetStaticMesh(SphereMeshAsset.Object);
}


void AMass::BeginPlay()
{
	Super::BeginPlay();
	mesh->SetWorldLocation(FVector(position.Y, position.X, 0.0));
}


void AMass::Tick(float DeltaSecs)
{
	Super::Tick(DeltaSecs);
	mesh->SetWorldLocation(FVector(position.Y, position.X, 0.0f));

	// Super::Tick(DeltaSecs);
	// position += velocity * DeltaSecs;
	// if (position.X > 2000.0f) {
	// 	position.X = -2000.0f;
	// }
	// mesh->SetWorldLocation(FVector(position.Y, position.X, 0.0f));

}


UStaticMeshComponent* AMass::GetMesh() {
	return mesh;
} 


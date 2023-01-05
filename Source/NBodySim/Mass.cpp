#include "Mass.h"


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
	mesh->SetWorldLocation(FVector(0.0, 0.0, 0.0));
}


void AMass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mesh->SetWorldLocation(mesh->GetRelativeLocation() + FVector(DeltaTime * 50.0, 0.0, 0.0));
	if (mesh->GetRelativeLocation().X > 100.0) {
		mesh->SetWorldLocation(FVector(-100.0, 0.0, 0.0));
	}
}


UStaticMeshComponent* AMass::GetMesh() {
	return mesh;
} 


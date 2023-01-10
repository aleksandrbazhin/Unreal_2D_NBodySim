#include "BodyManager.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"


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
	for (int32 Index = 0; Index < BodyNum; ++Index) {
		FVector2D RandomPosition(FMath::RandPointInCircle(PlacementRadius));
		FVector2D RandomVelocity {FMath::FRandRange(BaseInitialVelocity - 100.0f, BaseInitialVelocity + 100.0f), 0};
		RandomVelocity = RandomVelocity.GetRotated(90.0f + FMath::RadiansToDegrees(FMath::Atan2(RandomPosition.Y, RandomPosition.X)));
		float Mass = FMath::FRandRange(MinMass, MaxMass);
		float MeshScale = FMath::Sqrt(Mass) * BodyDisplayScale;
		FTransform MeshTransform (
			FRotator(),
			TranslationFrom2DCoordinates(RandomPosition),
			FVector(MeshScale, MeshScale, 1.0f)
		);
		Transforms[Index] = MeshTransform;
		Bodies[Index] = FBodyEntity{RandomPosition, RandomVelocity, Mass, Index};
	}
	InstancedMesh->AddInstances(Transforms, false);
}


void ABodyManager::BeginPlay()
{
	Super::BeginPlay();
	InitBodies();
}


void ABodyManager::GravityStep(float DeltaTime)
{
    ParallelFor(Bodies.Num(), [&] (int32 Index) {
        FVector2D Acceleration(0.0f, 0.0f);
        for (const FBodyEntity& AffectingBody: Bodies) {
            if (AffectingBody.Index == Bodies[Index].Index) continue; // exclude self
            float Distance = FVector2D::Distance(Bodies[Index].Position, AffectingBody.Position);
            Distance = FMath::Max(Distance, MinimumGravityDistance); // avoids division by zero
            Acceleration += AffectingBody.Mass / Distance * G / Distance * (AffectingBody.Position - Bodies[Index].Position) / Distance;
        }
        Bodies[Index].Velocity += Acceleration * DeltaTime ;
    });
}


void ABodyManager::UpdatePositionStep(float DeltaTime)
{
    FVector2D HalfWorld(WorldWidth * 0.5f, WorldHeight * 0.5f);
	for (FBodyEntity& Body: Bodies) {
		Body.Position += Body.Velocity * DeltaTime;
		Body.Position.X = FMath::Wrap(Body.Position.X, -HalfWorld.X, HalfWorld.X);
		Body.Position.Y = FMath::Wrap(Body.Position.Y, -HalfWorld.Y, HalfWorld.Y);
		Transforms[Body.Index].SetTranslation(TranslationFrom2DCoordinates(Body.Position));
	}
	InstancedMesh->BatchUpdateInstancesTransforms(0, Transforms, false, true);
}


void ABodyManager::Tick(float DeltaTime)
{
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

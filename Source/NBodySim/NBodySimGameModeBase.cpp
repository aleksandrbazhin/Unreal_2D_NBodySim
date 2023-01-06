#include "NBodySimGameModeBase.h"

// TODO: export numbodies as a parameter to editor
// TODO: determine world_size from camera, or camera rect from world size 

const FVector2D ANBodySimGameModeBase::WORLD_SIZE = FVector2D(8000, 4520);
const int ANBodySimGameModeBase::NUM_BODIES;


ANBodySimGameModeBase::ANBodySimGameModeBase() 
{
    PrimaryActorTick.bCanEverTick = true;
}


void ANBodySimGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    FRotator no_rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters spawn_info;

    for (int i = 0; i < ANBodySimGameModeBase::NUM_BODIES; i++) {
        FVector2D random_position(
            FMath::FRandRange(-ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f, ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f),
            FMath::FRandRange(-ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f, ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f)
        ); 
        // FVector location(random_position.Y, random_position.X, 0.0f);

        AMass* mass = GetWorld()->SpawnActor<AMass>(
            AMass::LocationFrom2DPosition(random_position), no_rotation, spawn_info);
        mass->Position = random_position;
        mass->Velocity = FVector2D(
            FMath::FRandRange(-400.0f, 400.0f),
            FMath::FRandRange(-400.0f, 400.0f)
        ); 
        masses.Push(mass);
    }
}

void ANBodySimGameModeBase::Tick(float DeltaSecs)
{
    Super::Tick(DeltaSecs);

    for (AMass* mass: masses)
    {
        mass->Position += mass->Velocity * DeltaSecs;

        if (mass->Position.X > ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f) {
            mass->Position.X = -ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f;
        }
        if (mass->Position.X < -ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f) {
            mass->Position.X = ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f;
        }
        if (mass->Position.Y > ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f) {
            mass->Position.Y = -ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f;
        }
        if (mass->Position.Y < -ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f) {
            mass->Position.Y = ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f;
        }
    }

}
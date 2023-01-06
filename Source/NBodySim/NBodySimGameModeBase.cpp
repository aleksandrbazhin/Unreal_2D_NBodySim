#include "NBodySimGameModeBase.h"

#include "Runtime/Core/Public/Async/ParallelFor.h"

// TODO: export numbodies as a parameter to editor
// TODO: determine world_size from camera, or camera rect from world size 


const FVector2D ANBodySimGameModeBase::WORLD_SIZE = FVector2D(8000, 4520);
const int ANBodySimGameModeBase::NUM_BODIES;
const float GRAVITY_CONSTANT = 10000.0f;
const float MINIMUM_AFFECTING_DISTANCE = 10.0f;
const float MAX_TICK = 0.0167;

ANBodySimGameModeBase::ANBodySimGameModeBase() 
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = false;
    PrimaryActorTick.TickGroup = TG_DuringPhysics;
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
        AMass* mass = GetWorld()->SpawnActor<AMass>(
            AMass::LocationFrom2DPosition(random_position), no_rotation, spawn_info);
        mass->Position = random_position;
        mass->Velocity = FVector2D(
            FMath::FRandRange(-400.0f, 400.0f),
            FMath::FRandRange(-400.0f, 400.0f)
        ); 
        // mass->Mass = FMath::FRandRange(10.0, 100.0);
        mass->SetMass(FMath::FRandRange(10.0, 100.0));
        Masses.Push(mass);
    }
}

void ANBodySimGameModeBase::Tick(float DeltaSecs)
{
    if (DeltaSecs > MAX_TICK) {
        DeltaSecs = MAX_TICK;
    }
    Super::Tick(DeltaSecs);

    FVector2D half_world = ANBodySimGameModeBase::WORLD_SIZE / 2.0;


    ParallelFor(Masses.Num(), [&] (int i) {
        FVector2D force(0.0, 0.0);
        for (AMass* affecting_mass: Masses) {
            if (affecting_mass == Masses[i]) continue; // exclude self
            float distance = FVector2D::Distance(Masses[i]->Position, affecting_mass->Position);
            distance = FMath::Clamp(distance, MINIMUM_AFFECTING_DISTANCE, half_world.X); // avoids division by zero
            force += Masses[i]->Mass * affecting_mass->Mass * GRAVITY_CONSTANT / distance / distance * (affecting_mass->Position - Masses[i]->Position) / distance;
        }
        Masses[i]->Velocity += force * DeltaSecs / Masses[i]->Mass;
    });


    // for (AMass* mass: Masses)
    // {
    //     FVector2D force(0.0, 0.0);
    //     for (AMass* affecting_mass: Masses) {
    //         if (affecting_mass == mass) continue; // exclude self
    //         float distance = FVector2D::Distance(mass->Position, affecting_mass->Position);
    //         distance = FMath::Clamp(distance, MINIMUM_AFFECTING_DISTANCE, WORLD_SIZE.X / 2.0f); // avoids division by zero
    //         force += mass->Mass * affecting_mass->Mass * GRAVITY_CONSTANT / distance / distance * (affecting_mass->Position - mass->Position) / distance;
    //     }
    //     mass->Velocity += force * DeltaSecs / mass->Mass; 
    // }

    for (AMass* mass: Masses)
    {
        mass->Position += mass->Velocity * DeltaSecs;
        mass->Position = FVector2D(
            FMath::Wrap(mass->Position.X, -half_world.X, half_world.X),
            FMath::Wrap(mass->Position.Y, -half_world.Y, half_world.Y)
        );
    }


}
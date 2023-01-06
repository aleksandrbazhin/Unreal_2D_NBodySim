// Copyright Epic Games, Inc. All Rights Reserved.


#include "NBodySimGameModeBase.h"
// #include ""

const FVector2D ANBodySimGameModeBase::WORLD_SIZE = FVector2D(8000, 4520);
const int ANBodySimGameModeBase::NUM_BODIES;

ANBodySimGameModeBase::ANBodySimGameModeBase() 
{
    PrimaryActorTick.bCanEverTick = true;
}


void ANBodySimGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    for (int i = 0; i < ANBodySimGameModeBase::NUM_BODIES; i++) {
        FVector2D random_position(
            FMath::FRandRange(-ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f, ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f),
            FMath::FRandRange(-ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f, ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f)
        ); 
        FVector location(random_position.Y, random_position.X, 0.0f);
        FRotator rotation(0.0f, 0.0f, 0.0f);
        FActorSpawnParameters spawn_info;
        AMass* mass = GetWorld()->SpawnActor<AMass>(location, rotation, spawn_info);
        mass->position = random_position;
        mass->velocity = FVector2D(
            FMath::FRandRange(-400.0f, 400.0f),
            FMath::FRandRange(-400.0f, 400.0f)
        ); 

        // mass->velocity = FVector2D(50.0, 0.0);
        masses.Push(mass);
    }
}

void ANBodySimGameModeBase::Tick(float DeltaSecs)
{
    Super::Tick(DeltaSecs);

    for (AMass* mass: masses)
    {
        mass->position += mass->velocity * DeltaSecs;

        if (mass->position.X > ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f) {
            mass->position.X = -ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f;
        }
        if (mass->position.X < -ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f) {
            mass->position.X = ANBodySimGameModeBase::WORLD_SIZE.X / 2.0f;
        }
        if (mass->position.Y > ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f) {
            mass->position.Y = -ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f;
        }
        if (mass->position.Y < -ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f) {
            mass->position.Y = ANBodySimGameModeBase::WORLD_SIZE.Y / 2.0f;
        }
    }

}
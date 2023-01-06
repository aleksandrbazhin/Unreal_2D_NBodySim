// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Mass.h"

#include "NBodySimGameModeBase.generated.h"



/**
 * 
 */
UCLASS()
class NBODYSIM_API ANBodySimGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANBodySimGameModeBase();
	virtual void Tick(float DeltaSecs) override;
	static const FVector2D WORLD_SIZE;
	static const int NUM_BODIES = 500;

protected:
	virtual void BeginPlay() override;

private:
	TArray<AMass*> masses;
};

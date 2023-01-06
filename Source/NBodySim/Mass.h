// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mass.generated.h"

UCLASS()
class NBODYSIM_API AMass : public AActor
{
	GENERATED_BODY()
	
public:
	AMass();
	double mass;
	FVector2D position;
	FVector2D velocity;

	
	UStaticMeshComponent* mesh;
	virtual void Tick(float DeltaSecs) override;
	UStaticMeshComponent* GetMesh(); 


protected:
	virtual void BeginPlay() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyManager.generated.h"


USTRUCT()
struct FMassEntity {
	GENERATED_BODY()

	FVector2D Position;
	FVector2D Velocity;
 	float Mass = 1.0f;
	int id;
};


UCLASS()
class NBODYSIM_API ABodyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABodyManager();
	UPROPERTY(VisibleAnywhere, Instanced, NoClear)
		UInstancedStaticMeshComponent *InstancedMesh;
	UPROPERTY(EditAnywhere)
		int MassCount = 100;

private:
	TArray<FMassEntity> Masses;
	void InitMasses();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};

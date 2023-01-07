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
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		int MassCount = 100;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float BodyDisplayScale = 0.003f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float G = 10000.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MaxInitialVelocity = 400.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MinMass = 20.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MaxMass = 100.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Ortho Camera")
		float WorldWidth = 8000.0f;
		float WorldHeight = WorldWidth / 1.777778f; // camera default aspect ratio


private:
	TArray<FMassEntity> Masses;
	TArray<FTransform> Transforms;
	void InitMasses();
	void RunGravityStep(float DeltaTime);

	static FVector PositionFromPlanar(const FVector2D& XYCoordinates);


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};

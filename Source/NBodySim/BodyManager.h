// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BodyManager.generated.h"


USTRUCT()
struct FBodyEntity {
	GENERATED_BODY()

	UPROPERTY()
		FVector2D Position;
	UPROPERTY()
		FVector2D Velocity;
	UPROPERTY()
		float Mass = 1.0f;
	UPROPERTY()
		int32 Index;
};


UCLASS()
class NBODYSIM_API ABodyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABodyManager();
	UPROPERTY(VisibleAnywhere, Instanced, NoClear)
		UInstancedStaticMeshComponent* InstancedMesh;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		bool IsGravityEnabled = true;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		int BodyNum = 2500;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float PlacementRadius = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float BaseInitialVelocity = 500.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float BodyDisplayScale = 0.02f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float G = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MinMass = 20.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MaxMass = 100.0f;
	UPROPERTY(EditAnywhere, Category = "NBody Simulation Parameters")
		float MinimumGravityDistance = 100.0f;  // prevents division by zero and forces too high	
	UPROPERTY(EditAnywhere, Category = "NBody Ortho Camera")
		float WorldWidth = 8000.0f;
		float WorldHeight = WorldWidth / 1.777778f; // camera default aspect ratio


private:
	UPROPERTY()
		TArray<FBodyEntity> Bodies;
	UPROPERTY()
		TArray<FTransform> Transforms;
	// void CreateBody(FVector2D Position, FVector2D Velocity, float Mass, )
	void InitBodies();
	void BuildBHTree();
	void GravityStep(float DeltaTime);
	void UpdatePositionStep(float DeltaTime);

	static FVector TranslationFrom2DCoordinates(const FVector2D& XYCoordinates);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
